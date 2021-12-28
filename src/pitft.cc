#include <nan.h>
#include "framebuffer.h"

using namespace v8;

NAN_METHOD(CreateObject) {
  Nan::HandleScope scope;
  info.GetReturnValue().Set(FrameBuffer::NewInstance(info[0], info[1]));
}

void InitAll(Local<Object> exports, Local<Object> module) {
  Nan::HandleScope scope;

  FrameBuffer::Init();

  Local<Context> context = exports->CreationContext();
  module->Set(context,
              Nan::New("exports").ToLocalChecked(),
              Nan::New<FunctionTemplate>(CreateObject)
                  ->GetFunction(context).ToLocalChecked()).ToChecked();
}

NODE_MODULE(pitft, InitAll)
