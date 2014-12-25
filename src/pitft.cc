#include <nan.h>
#include "framebuffer.h"

using namespace v8;

NAN_METHOD(CreateObject) {
  NanScope();

  NanReturnValue(FrameBuffer::NewInstance(args));
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
    FrameBuffer::Init();

    module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(CreateObject)->GetFunction());
}

NODE_MODULE(pitft, InitAll)
