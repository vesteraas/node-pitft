#include "framebuffer.h"

using namespace v8;

Nan::Persistent<Function> FrameBuffer::constructor;

void FrameBuffer::Init() {
    Nan::HandleScope scope;

    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(FrameBuffer::New);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(Nan::New("FrameBuffer").ToLocalChecked());

    ctor->PrototypeTemplate()->Set(Nan::New("size").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Size));
    ctor->PrototypeTemplate()->Set(Nan::New("data").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Data));
    ctor->PrototypeTemplate()->Set(Nan::New("clear").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Clear));
    ctor->PrototypeTemplate()->Set(Nan::New("blit").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Blit));
    ctor->PrototypeTemplate()->Set(Nan::New("color").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Color));
    ctor->PrototypeTemplate()->Set(Nan::New("fill").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Fill));
    ctor->PrototypeTemplate()->Set(Nan::New("line").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Line));
    ctor->PrototypeTemplate()->Set(Nan::New("rect").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Rect));
    ctor->PrototypeTemplate()->Set(Nan::New("circle").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Circle));
    ctor->PrototypeTemplate()->Set(Nan::New("font").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Font));
    ctor->PrototypeTemplate()->Set(Nan::New("text").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Text));
    ctor->PrototypeTemplate()->Set(Nan::New("image").ToLocalChecked(),
      Nan::New<FunctionTemplate>(Image));
    ctor->PrototypeTemplate()->Set(Nan::New("patternCreateLinear").ToLocalChecked(),
      Nan::New<FunctionTemplate>(PatternCreateLinear));
    ctor->PrototypeTemplate()->Set(Nan::New("patternCreateRGB").ToLocalChecked(),
      Nan::New<FunctionTemplate>(PatternCreateRGB));
    ctor->PrototypeTemplate()->Set(Nan::New("patternAddColorStop").ToLocalChecked(),
      Nan::New<FunctionTemplate>(PatternAddColorStop));
    ctor->PrototypeTemplate()->Set(Nan::New("patternDestroy").ToLocalChecked(),
      Nan::New<FunctionTemplate>(PatternDestroy));

    constructor.Reset(Nan::GetFunction(ctor).ToLocalChecked());

}

Local<Object> FrameBuffer::NewInstance(Local<Value> arg, Local<Value> arg2) {

  Nan::EscapableHandleScope scope;

  const unsigned argc = 2;
  Local<Value> argv[argc] = { arg, arg2 };
  Local<Function> cons = Nan::New<Function>(constructor);
  Local<Object> instance = Nan::NewInstance(cons, argc, argv).ToLocalChecked();

  return scope.Escape(instance);

}

NAN_METHOD(FrameBuffer::New) {
    Nan::HandleScope scope;

    Nan::Utf8String path(info[0]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>()));
    std::string _path = std::string(*path);

    FrameBuffer *obj = new FrameBuffer(_path.c_str());
    obj->drawToBuffer = info[1]->IsUndefined() ? false : Nan::To<bool>(info[1]).FromJust();

    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(FrameBuffer::Size) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    Local<Object> sizeObject = Nan::New<Object>();

    Nan::Set(sizeObject, Nan::New("width").ToLocalChecked(), Nan::New(obj->vinfo.xres));
    Nan::Set(sizeObject, Nan::New("height").ToLocalChecked(), Nan::New(obj->vinfo.yres));

    info.GetReturnValue().Set(sizeObject);
}

NAN_METHOD(FrameBuffer::Data) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    Local<Object> bufferObject = Nan::NewBuffer(obj->fbp, obj->screenSize).ToLocalChecked();

    info.GetReturnValue().Set(bufferObject);
}

NAN_METHOD(FrameBuffer::Clear) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    cairo_destroy(cr);

    return;
}

NAN_METHOD(FrameBuffer::Blit) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    if (obj->drawToBuffer) {
        cairo_t *cr = cairo_create (obj->screenSurface);
        cairo_set_source_surface (cr, obj->bufferSurface, 0, 0);
        cairo_paint (cr);

        cairo_destroy(cr);
    }

    return;
}

NAN_METHOD(FrameBuffer::Color) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    if (info[1]->IsUndefined()) {
        obj->usedPattern = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
        obj->usePattern = true;
    } else {
        obj->r = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
        obj->g = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());
        obj->b = (info[2]->NumberValue(Nan::GetCurrentContext()).FromJust());
        obj->usePattern = false;
    }

    return;
}

NAN_METHOD(FrameBuffer::PatternCreateLinear) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    double x0, y0, x1, y1;
    size_t pos;

    if (info[4]->IsUndefined()) {
        x0 = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
        y0 = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());
        x1 = (info[2]->NumberValue(Nan::GetCurrentContext()).FromJust());
        y1 = (info[3]->NumberValue(Nan::GetCurrentContext()).FromJust());
        obj->pattern.push_back(cairo_pattern_create_linear(x0, y0, x1, y1));
        pos = obj->pattern.size() - 1;
    } else {
        pos = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
        x0 = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());
        y0 = (info[2]->NumberValue(Nan::GetCurrentContext()).FromJust());
        x1 = (info[3]->NumberValue(Nan::GetCurrentContext()).FromJust());
        y1 = (info[4]->NumberValue(Nan::GetCurrentContext()).FromJust());

        while (obj->pattern.size() <= pos) {
            obj->pattern.push_back(nullptr);
        }

        if (obj->pattern[pos] != nullptr) {
            cairo_pattern_destroy(obj->pattern[pos]);
        }

        obj->pattern[pos] = cairo_pattern_create_linear(x0, y0, x1, y1);
    }

    info.GetReturnValue().Set(pos);
}

NAN_METHOD(FrameBuffer::PatternCreateRGB) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    double r, g, b, a;
    size_t pos;

    if (info[4]->IsUndefined()) {
        r = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
        g = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());
        b = (info[2]->NumberValue(Nan::GetCurrentContext()).FromJust());
        a = info[3]->IsUndefined() ? 1 : (info[3]->NumberValue(Nan::GetCurrentContext()).FromJust());
        obj->pattern.push_back(cairo_pattern_create_rgba(r, g, b, a));
        pos = obj->pattern.size() - 1;
    } else {
        pos = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
        r = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());
        g = (info[2]->NumberValue(Nan::GetCurrentContext()).FromJust());
        b = (info[3]->NumberValue(Nan::GetCurrentContext()).FromJust());
        a = info[4]->IsUndefined() ? 1 : (info[4]->NumberValue(Nan::GetCurrentContext()).FromJust());

        while (obj->pattern.size() <= pos) {
            obj->pattern.push_back(nullptr);
        }

        if (obj->pattern[pos] != nullptr) {
            cairo_pattern_destroy(obj->pattern[pos]);
        }

        obj->pattern[pos] = cairo_pattern_create_rgba(r, g, b, a);
    }

    info.GetReturnValue().Set(pos);
}

NAN_METHOD(FrameBuffer::PatternAddColorStop) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    size_t patternIndex = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());

    double offset = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double r = (info[2]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double g = (info[3]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double b = (info[4]->NumberValue(Nan::GetCurrentContext()).FromJust());

    if (!info[5]->IsUndefined()) {
        double alpha = info[5]->IsUndefined() ? 1 : info[5]->NumberValue(Nan::GetCurrentContext()).FromJust();
        cairo_pattern_add_color_stop_rgba(obj->pattern[patternIndex], offset, r, g, b, alpha);
    } else {
        cairo_pattern_add_color_stop_rgb(obj->pattern[patternIndex], offset, r, g, b);
    }

    return;
}

NAN_METHOD(FrameBuffer::PatternDestroy) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    size_t patternIndex = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());

    cairo_pattern_destroy(obj->pattern[patternIndex]);

    obj->pattern[patternIndex] = nullptr;

    return;
}

#define cairoSetSourceMacro(cr, obj) \
    if (obj->usePattern) { \
        if (obj->pattern.size() <= obj->usedPattern) { \
            Nan::ThrowError("Error using pattern, index not exists"); return; \
        } \
        if (obj->pattern[obj->usedPattern] == nullptr) { \
            Nan::ThrowError("Error using pattern, pattern is destroyed"); return; \
        } \
        if (cairo_pattern_status(obj->pattern[obj->usedPattern]) != CAIRO_STATUS_SUCCESS) { \
            Nan::ThrowError("Error using pattern, pattern status invalid"); return; \
        } \
        cairo_set_source(cr, obj->pattern[obj->usedPattern]); \
    } else { \
        cairo_set_source_rgb(cr, obj->r, obj->g, obj->b); \
    }

NAN_METHOD(FrameBuffer::Fill) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairoSetSourceMacro(cr, obj);
    cairo_paint(cr);

    cairo_destroy(cr);

    return;
}

NAN_METHOD(FrameBuffer::Line) {
    Nan::HandleScope scope;

    double x0 = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double y0 = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double x1 = (info[2]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double y1 = (info[3]->NumberValue(Nan::GetCurrentContext()).FromJust());

    double w = info[4]->IsUndefined() ? 1 : info[4]->NumberValue(Nan::GetCurrentContext()).FromJust();

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairoSetSourceMacro(cr, obj);

    cairo_move_to(cr, x0, y0);
    cairo_line_to(cr, x1, y1);

    cairo_set_line_width(cr, w);
    cairo_stroke(cr);

    cairo_destroy(cr);

    return;
}

NAN_METHOD(FrameBuffer::Rect) {
    Nan::HandleScope scope;

    double x = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double y = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double w = (info[2]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double h = (info[3]->NumberValue(Nan::GetCurrentContext()).FromJust());

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairoSetSourceMacro(cr, obj);

    cairo_rectangle(cr, x, y, w, h);

    if (!info[4]->IsUndefined() && Nan::To<bool>(info[5]).FromJust() == false) {
        double w = info[5]->IsUndefined() ? 1 : info[5]->NumberValue(Nan::GetCurrentContext()).FromJust();
        cairo_set_line_width(cr, w);
        cairo_stroke(cr);
    } else if (!info[4]->IsUndefined() && Nan::To<bool>(info[4]).FromJust() == true) {
        cairo_fill(cr);
    } else {
        cairo_fill(cr);
    }

    cairo_destroy(cr);

    return;
}

NAN_METHOD(FrameBuffer::Circle) {
    Nan::HandleScope scope;

    double x = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double y = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double radius = (info[2]->NumberValue(Nan::GetCurrentContext()).FromJust());

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairoSetSourceMacro(cr, obj);

    cairo_arc(cr, x, y, radius, 0, 2*3.141592654);

    if (!info[3]->IsUndefined() && Nan::To<bool>(info[3]).FromJust() == false) {
        double w = info[4]->IsUndefined() ? 1 : info[4]->NumberValue(Nan::GetCurrentContext()).FromJust();
        cairo_set_line_width(cr, w);
        cairo_stroke(cr);
    } else if (!info[3]->IsUndefined() && Nan::To<bool>(info[3]).FromJust() == true) {
        cairo_fill(cr);
    } else {
        cairo_fill(cr);
    }

    cairo_destroy(cr);

    return;
}

NAN_METHOD(FrameBuffer::Font) {
    Nan::HandleScope scope;

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    Nan::Utf8String fontName(info[0]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>()));
    std::string _fontName = std::string(*fontName);

    obj->fontName = _fontName.c_str();
    obj->fontSize = info[1]->IsUndefined() ? 12 : info[1]->NumberValue(Nan::GetCurrentContext()).FromJust();
    obj->fontBold = info[2]->IsUndefined() ? false : Nan::To<bool>(info[2]).FromJust();

    return;
}

NAN_METHOD(FrameBuffer::Text) {
    Nan::HandleScope scope;

    double x = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double y = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());

    Nan::Utf8String text(info[2]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>()));
    std::string _text = std::string(*text);

    bool textCentered = info[3]->IsUndefined() ? false : Nan::To<bool>(info[3]).FromJust();
    double textRotation = info[4]->IsUndefined() ? 0 : info[4]->NumberValue(Nan::GetCurrentContext()).FromJust();
    bool textRight = info[5]->IsUndefined() ? false : Nan::To<bool>(info[5]).FromJust();

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairoSetSourceMacro(cr, obj);

    if (obj->fontBold) {
        cairo_select_font_face(cr, obj->fontName, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    } else {
        cairo_select_font_face(cr, obj->fontName, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    }

    cairo_set_font_size(cr, obj->fontSize);

    cairo_translate(cr, x, y);

    if (textRotation != 0) {
        cairo_rotate(cr, textRotation / (180.0 / 3.141592654));
    }

    if (textCentered) {
        cairo_text_extents_t extents;
        cairo_text_extents(cr, _text.c_str(), &extents);

        cairo_move_to(cr, -extents.width/2, extents.height/2);
    } else if (textRight) {
        cairo_text_extents_t extents;
        cairo_text_extents(cr, _text.c_str(), &extents);

        cairo_move_to(cr, -extents.width, 0);
    }

    cairo_show_text(cr, _text.c_str());

    cairo_destroy(cr);

    return;
}

NAN_METHOD(FrameBuffer::Image) {
    Nan::HandleScope scope;

    double x = (info[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
    double y = (info[1]->NumberValue(Nan::GetCurrentContext()).FromJust());

    Nan::Utf8String path(info[2]->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>()));
    std::string _path = std::string(*path);

    FrameBuffer *obj = Nan::ObjectWrap::Unwrap<FrameBuffer>(info.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairo_surface_t *image = cairo_image_surface_create_from_png(_path.c_str());
    cairo_set_source_surface(cr, image, x, y);
    cairo_paint(cr);

    cairo_status_t status = cairo_status(cr);

    if (status != CAIRO_STATUS_SUCCESS) {
        Nan::ThrowError("Error reading image");
    }

    cairo_surface_destroy(image);
    cairo_destroy(cr);

    return;
}

cairo_t* FrameBuffer::getDrawingContext(FrameBuffer *obj) {
    if (obj->drawToBuffer) {
        return cairo_create(obj->bufferSurface);
    } else {
        return cairo_create(obj->screenSurface);
    }
}

FrameBuffer::FrameBuffer(const char *path) {
    fbfd = open(path, O_RDWR);
    if (fbfd == -1) {
        Nan::ThrowError("Error opening framebuffer device");
        return;
    }

    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        Nan::ThrowError("Error retrieving data from framebuffer");
        return;
    }

    memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

    vinfo.bits_per_pixel = 8;
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)) {
        Nan::ThrowError("Error sending data to framebuffer");
        return;
    }

    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        Nan::ThrowError("Error retrieving data from framebuffer");
        return;
    }

    screenSize = finfo.smem_len;
    fbp = (char*)mmap(0,
                      screenSize,
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED,
                      fbfd,
                      0);

    bbp = (char *)malloc(screenSize);

    if ((int)fbp == -1) {
        Nan::ThrowError("Error during memory mapping");
        return;
    }

    bufferSurface = cairo_image_surface_create_for_data ((unsigned char *)bbp, CAIRO_FORMAT_RGB16_565, vinfo.xres, vinfo.yres, cairo_format_stride_for_width(CAIRO_FORMAT_RGB16_565, vinfo.xres));

    if (cairo_surface_status(bufferSurface) != CAIRO_STATUS_SUCCESS) {
        Nan::ThrowError("Error creating buffer surface");
        return;
    }

    screenSurface = cairo_image_surface_create_for_data ((unsigned char *)fbp, CAIRO_FORMAT_RGB16_565, vinfo.xres, vinfo.yres, cairo_format_stride_for_width(CAIRO_FORMAT_RGB16_565, vinfo.xres));

    if (cairo_surface_status(bufferSurface) != CAIRO_STATUS_SUCCESS) {
        Nan::ThrowError("Error creating screeh surface");
    }
}

FrameBuffer::~FrameBuffer() {
    size_t patternSize = pattern.size();
    for(size_t i = 0; i < patternSize; i++) {
      if (pattern[i] != nullptr) {
        cairo_pattern_destroy(pattern[i]);
      }
    }

    if ((int)fbp != -1) {
        free(bbp);
        munmap(fbp, screenSize);

        if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) {
            Nan::ThrowError("Error restoring framebuffer state");
        }
    }

    if (fbfd != -1) {
        close(fbfd);
    }

    if (cairo_surface_status(bufferSurface) == CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(bufferSurface);
    }

    if (cairo_surface_status(screenSurface) == CAIRO_STATUS_SUCCESS) {
        cairo_surface_destroy(screenSurface);
    }
}
