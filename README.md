# Alpha Wrap for Javascript
Port CGAL::alpha_shape_3 to Javascript using Emscripten.

## Demo
[https://petingo.cc/alpha-wrap-js/example/](https://petingo.cc/alpha-wrap-js/example/)

![](ScreenRecording.gif)

## Prerequisites

- [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html) installed and activated
- GMP and MPFR compiled with Emscripten (paths configured in `CMakeLists.txt` as `gmp-em` / `mpfr-em`)
- GMP and MPFR compiled natively (paths configured in `CMakeLists.txt` as `gmp-c` / `mpfr-c`)
- Boost headers (header-only, path configured in `CMakeLists.txt`)
- CGAL 5.6 headers (path configured in `CMakeLists.txt`)

## First-time setup

### 1. Generate WebIDL glue code

Only needed once, or whenever `src/WebIDL/alpha_wrap.idl` changes. Run from the project root:
```
source /path/to/emsdk/emsdk_env.sh
python3 $EMSDK/upstream/emscripten/tools/webidl_binder.py \
    src/WebIDL/alpha_wrap.idl src/WebIDL/alpha_wrap_glue
```
This generates `src/WebIDL/alpha_wrap_glue.cpp` and `src/WebIDL/alpha_wrap_glue.js`.

### 2. Configure the Emscripten (WASM) build

Only needed once (or after changing `CMakeLists.txt`). Run from the project root:
```
source /path/to/emsdk/emsdk_env.sh
mkdir -p build && cd build
emcmake cmake ..
```

## Build for web (WASM)

```
source /path/to/emsdk/emsdk_env.sh
cd build
cmake --build . --target alpha_wrap
```

This produces `build/alpha_wrap.js` and `build/alpha_wrap.wasm`. Copy them to the example:
```
cp build/alpha_wrap.js build/alpha_wrap.wasm example/scripts/
```

## Run the C++ test (native)

```
mkdir -p cmake-build-debug && cd cmake-build-debug
cmake ..
cmake --build .
./alpha_wrap_test
```

## Run the example web page
```
cd example
npx server ./
```
Then open http://localhost:8000 in your browser.

## Use from another website

The compiled files are deployed to GitHub Pages alongside the demo. You can import the library directly from any website without copying the WASM binary — it loads automatically from the same GitHub Pages origin.

### Import map

Add this to your HTML `<head>` before any module scripts:

```html
<script type="importmap">
  { "imports": { "alpha-wrap-js": "https://petingo.github.io/alpha-wrap-js/example/alpha-wrap-lib.js" } }
</script>
```

### Usage

```javascript
import { createAlphaWrap } from 'alpha-wrap-js';

const wrap = await createAlphaWrap(10, 300);  // alpha, offset

wrap.addPoint(x, y, z);  // add all your points
wrap.wrap();              // run the algorithm
const ply = wrap.getWrappedMeshPly();  // get result as PLY string
```

`alpha_wrap.wasm` is fetched automatically from GitHub Pages alongside the JS — no extra configuration needed.

---

## Using the WASM module in a web app

The build outputs an ES6 module (`alpha_wrap.js`) that wraps the WASM binary. To use it:

### 1. Serve both files from the same directory

`alpha_wrap.js` and `alpha_wrap.wasm` must be served together (e.g. both in `example/scripts/`).

### 2. Reference it via an import map

In your HTML, declare an import map so you can import it by name:
```html
<script type="importmap">
{
  "imports": {
    "AlphaWrap": "./scripts/alpha_wrap.js"
  }
}
</script>
<script type="module" src="scripts/main.js"></script>
```

> **Note:** Use a relative path (`./scripts/alpha_wrap.js`), not an absolute path (`/scripts/alpha_wrap.js`), so it works on both `localhost` and hosted subdirectories (e.g. GitHub Pages).

### 3. Load the module and instantiate

```js
import { default as createAlphaWrapModule } from 'AlphaWrap';

createAlphaWrapModule().then(AlphaWrap => {
    const alphaWrap = new AlphaWrap.AlphaWrap(alpha, offset);

    // Add points
    alphaWrap.addPoint(x, y, z);

    // Run the wrap
    alphaWrap.wrap();

    // Get the result as a PLY string
    const plyString = alphaWrap.getWrappedMeshPly();
});
```

See `example/scripts/main.js` for a full working example with three.js.

## CGAL Fixes for WebAssembly

Porting CGAL to WebAssembly via Emscripten required solving several issues related to floating-point predicate evaluation. Below is a summary of the problems encountered and how they were resolved.

### Problem: `orientation()` returns wrong results in WASM

CGAL's `Exact_predicates_inexact_constructions_kernel` (EPICK) relies on a filtered predicate chain:

1. **Static filter** — fast compile-time error bound check
2. **Interval filter** (`Interval_nt`) — uses FPU rounding mode control to compute tight upper/lower bounds
3. **Exact fallback** — arbitrary-precision arithmetic via GMP/MPFR

The interval filter (step 2) switches the FPU to **round-upward** mode to compute upper bounds and **round-downward** for lower bounds. WebAssembly does **not support FPU rounding mode control** — it always uses round-to-nearest. This means:

- Intervals computed in WASM are too tight (both bounds are round-to-nearest instead of being widened)
- Near-zero determinants get a collapsed interval like `[-3.73e-09; -3.73e-09]` instead of the correct `[-3.73e-08; 2.98e-08]`
- The filter incorrectly certifies a sign, skipping the exact fallback
- This causes `orientation()` to return wrong results, corrupting the Delaunay triangulation

### Solution: `CGAL_ALWAYS_ROUND_TO_NEAREST`

CGAL provides the `CGAL_ALWAYS_ROUND_TO_NEAREST` compile flag for environments without rounding mode control. When enabled, `IA_up()` in `FPU.h` uses `nextafter(d, infinity)` instead of relying on the hardware rounding mode:

```cpp
// FPU.h — with CGAL_ALWAYS_ROUND_TO_NEAREST
inline double IA_up(double d) {
    return nextafter(d, std::numeric_limits<double>::infinity());
}
```

This produces wider (conservative) intervals that always contain the true value. When the interval contains zero, the filter correctly returns UNCERTAIN and falls back to exact computation.

### Solution: Enable C++ exception handling in Emscripten

CGAL's `Filtered_predicate` uses a try/catch mechanism: when the interval filter can't determine a sign, it throws `Uncertain_conversion_exception`, which is caught to trigger the exact fallback. Emscripten disables C++ exception handling by default, which causes an abort instead of a fallback.

Fix: add `-fexceptions` to compile flags and `-sNO_DISABLE_EXCEPTION_CATCHING` to link flags.

### CMakeLists.txt configuration

These defines must be set **only for the Emscripten build** (not for native builds, which have proper FPU support):

```cmake
if (DEFINED EMSCRIPTEN)
    add_definitions(-DCGAL_DISABLE_ROUNDING_MATH_CHECK)
    add_definitions(-DCGAL_ALWAYS_ROUND_TO_NEAREST)
    # ...
    set_target_properties(alpha_wrap PROPERTIES COMPILE_FLAGS "... -fexceptions ...")
    set_target_properties(alpha_wrap PROPERTIES LINK_FLAGS "... -sNO_DISABLE_EXCEPTION_CATCHING ...")
endif()
```

Do **not** define `CGAL_ALWAYS_ROUND_TO_NEAREST` globally — it changes interval arithmetic behavior for native builds and can cause precondition failures there.

Also do **not** manually define `CGAL_USE_SSE2` — CGAL auto-detects it, and it conflicts with `CGAL_ALWAYS_ROUND_TO_NEAREST` (see `FPU.h` line ~120).

### Problem: Invalid inline asm constraint `+x` in WASM

CGAL's `FPU.h` uses inline assembly to prevent the compiler from optimizing away floating-point operations (a memory/register barrier). On x86 with SSE2, it uses the `+x` constraint which targets SSE registers:

```cpp
// FPU.h — original
# ifdef CGAL_HAS_SSE2
  asm volatile ("" : "+x"(x) );  // +x = SSE register constraint
```

The Emscripten/WASM target does not support the `+x` register constraint, causing a compile error:

```
error: invalid output constraint '+x' in asm
```

**Fix:** Replace `+x` with `+m` (memory constraint), which is valid on all targets and still prevents the compiler from optimizing across the barrier:

```cpp
# ifdef CGAL_HAS_SSE2
  asm volatile ("" : "+m"(x) );  // +m = memory constraint, works in WASM
```

## Reference
- [Official tutorial](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/WebIDL-Binder.html)
- [Another tutorial (Chinese)](https://874656645.github.io/posts/159-emscripten-%E4%BD%BF%E7%94%A8-webidl-binder-%E7%BB%91%E5%AE%9A-c++-%E4%BB%A3%E7%A0%81/)
- [GMP, MPFR](https://stackoverflow.com/questions/41080815/compiling-gmp-mpfr-with-emscripten)
- [Export as ES6 module](https://stackoverflow.com/questions/53309095/how-load-an-emscripten-generated-module-with-es6-import)
- [three.js setup template](https://medium.com/@ashabb/threejs-fullscreen-and-resizing-6aa84e21f407)