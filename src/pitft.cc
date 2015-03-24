#include <nan.h>
#include "framebuffer.h"

using namespace v8;

NAN_METHOD(CreateObject) {
  NanScope();
  NanReturnValue(FrameBuffer::NewInstance(args[0], args[1]));
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
  NanScope();

  FrameBuffer::Init();

  module->Set(NanNew("exports"),
      NanNew<FunctionTemplate>(CreateObject)->GetFunction());
}

NODE_MODULE(pitft, InitAll)
