
#include <emscripten.h>
#include <stdlib.h>

EM_JS_DEPS(webidl_binder, "$intArrayFromString,$UTF8ToString,$alignMemory");

extern "C" {

// Define custom allocator functions that we can force export using
// EMSCRIPTEN_KEEPALIVE.  This avoids all webidl users having to add
// malloc/free to -sEXPORTED_FUNCTIONS.
EMSCRIPTEN_KEEPALIVE void webidl_free(void* p) { free(p); }
EMSCRIPTEN_KEEPALIVE void* webidl_malloc(size_t len) { return malloc(len); }


// Interface: VoidPtr


void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// Interface: AlphaWrap


AlphaWrap* EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap_AlphaWrap_0() {
  return new AlphaWrap();
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap_wrap_1(AlphaWrap* self, char* serializedPoints) {
  return self->wrap(serializedPoints);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap___destroy___0(AlphaWrap* self) {
  delete self;
}

}

