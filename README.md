# Alpha Wrap for Javascript
Port CGAL::alpha_shape_3 to Javascript using Emscripten

- official tutorial: https://emscripten.org/docs/porting/connecting_cpp_and_javascript/WebIDL-Binder.html

```
python3 /home/wsl/emscripten/tools/webidl_binder.py ./alpha_wrap.idl alpha_wrap_glue
emcmake cmake ..
```
