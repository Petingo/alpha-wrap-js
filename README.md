# Alpha Wrap for Javascript
Port CGAL::alpha_shape_3 to Javascript using Emscripten

(everything is under construction)

## Run the script
```
python3 /home/wsl/emscripten/tools/webidl_binder.py ./alpha_wrap.idl alpha_wrap_glue

emcmake cmake ..
cmake --build . --target alpha_wrap
```

## Reference
- [Official tutorial](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/WebIDL-Binder.html)
- [Another tutorial (Chinese)](https://874656645.github.io/posts/159-emscripten-%E4%BD%BF%E7%94%A8-webidl-binder-%E7%BB%91%E5%AE%9A-c++-%E4%BB%A3%E7%A0%81/)