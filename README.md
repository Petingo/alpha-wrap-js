# Alpha Wrap for Javascript
Port CGAL::alpha_shape_3 to Javascript using Emscripten

(everything is under construction)

## Build for first time
```
python3 /home/wsl/emscripten/tools/webidl_binder.py ./alpha_wrap.idl alpha_wrap_glue



cmake --build . --target alpha_wrap
```

## Build for web
```
cd build
cmake --build . --target alpha_wrap
```
`alpha_wrap.js` and `alpha_wrap.wasm` will be generated.


## Note
- The orientation() return different value in WebAssembly
  - `CGAL/predicates/kernel_ftC3.h`
  - In `CGAL/Interval_nt.h`, the calculation of determinant() seems to produce different result
    - Local: 
    ```
    [852.111;852.111] [-0;-0] [0;-0]
    [0;0] [-852.111;-852.111] [0;0]
    [-29850.6;-29850.6] [29850.6;29850.6] [-0;0]
    m0123 = [-3.72529e-08;2.98023e-08]
    ```
    
    - WebAssembly:
    ```
    [852.111;852.111] [-0;0] [-0;0]
    [-0;0] [-852.111;-852.111] [-0;0]
    [-29850.6;-29850.6] [29850.6;29850.6] [-0;0]
    m0123 = [-3.72529e-09;-3.72529e-09]
    ```
  - In `FPH.h`
  ```
  inline double IA_opacify(double x)
  {
  #ifdef __llvm__
  // LLVM's support for inline asm is completely messed up:
  // https://bugs.llvm.org/show_bug.cgi?id=17958
  // https://bugs.llvm.org/show_bug.cgi?id=17959
  // etc.
  // This seems to produce code that is ok (not optimal but better than
  // volatile). In case of trouble, use volatile instead.
  
  # ifdef CGAL_HAS_SSE2
  asm volatile ("" : "+x"(x) );
  # elif (defined __VFP_FP__ && !defined __SOFTFP__) || defined __aarch64__
  // ARM
  asm volatile ("" : "+w"(x) );
  # else
  // asm volatile ("" : "+m"(x) ); // comment out this line improves
  # endif
  return x;
  ```

https://emscripten.org/docs/porting/simd.html

## Reference
- [Official tutorial](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/WebIDL-Binder.html)
- [Another tutorial (Chinese)](https://874656645.github.io/posts/159-emscripten-%E4%BD%BF%E7%94%A8-webidl-binder-%E7%BB%91%E5%AE%9A-c++-%E4%BB%A3%E7%A0%81/)
- [GMP, MPFR](https://stackoverflow.com/questions/41080815/compiling-gmp-mpfr-with-emscripten)
- [Export as ES6 module](https://stackoverflow.com/questions/53309095/how-load-an-emscripten-generated-module-with-es6-import)
- [three.js setup template](https://medium.com/@ashabb/threejs-fullscreen-and-resizing-6aa84e21f407)