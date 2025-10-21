
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

AlphaWrap* EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap_AlphaWrap_2(float alpha, float offset) {
  return new AlphaWrap(alpha, offset);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap_setAlpha_1(AlphaWrap* self, float alpha) {
  self->setAlpha(alpha);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap_setOffset_1(AlphaWrap* self, float offset) {
  self->setOffset(offset);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap_addPoint_3(AlphaWrap* self, float x, float y, float z) {
  self->addPoint(x, y, z);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap_clearPoints_0(AlphaWrap* self) {
  self->clearPoints();
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap_wrap_0(AlphaWrap* self) {
  return self->wrap();
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap_getWrappedMeshPly_0(AlphaWrap* self) {
  return self->getWrappedMeshPly();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_AlphaWrap___destroy___0(AlphaWrap* self) {
  delete self;
}

}

