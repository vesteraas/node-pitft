#include <nan.h>
#include "framebuffer.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
    FrameBuffer::Init(exports);
}

NODE_MODULE(addon, InitAll)
