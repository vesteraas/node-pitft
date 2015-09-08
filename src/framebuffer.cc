#include "framebuffer.h"

using namespace v8;

Persistent<Function> FrameBuffer::constructor;

void FrameBuffer::Init() {
    NanScope();

    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(FrameBuffer::New);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("FrameBuffer"));

    ctor->PrototypeTemplate()->Set(NanNew("size"),
      NanNew<FunctionTemplate>(Size)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("data"),
      NanNew<FunctionTemplate>(Data)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("clear"),
      NanNew<FunctionTemplate>(Clear)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("blit"),
      NanNew<FunctionTemplate>(Blit)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("color"),
      NanNew<FunctionTemplate>(Color)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("fill"),
      NanNew<FunctionTemplate>(Fill)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("line"),
      NanNew<FunctionTemplate>(Line)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("rect"),
      NanNew<FunctionTemplate>(Rect)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("circle"),
      NanNew<FunctionTemplate>(Circle)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("font"),
      NanNew<FunctionTemplate>(Font)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("text"),
      NanNew<FunctionTemplate>(Text)->GetFunction());
    ctor->PrototypeTemplate()->Set(NanNew("image"),
      NanNew<FunctionTemplate>(Image)->GetFunction());

    NanAssignPersistent(constructor, ctor->GetFunction());

}

Local<Object> FrameBuffer::NewInstance(Local<Value> arg, Local<Value> arg2) {

  NanEscapableScope();

  const unsigned argc = 2;
  Local<Value> argv[argc] = { arg, arg2 };
  Local<Function> cons = NanNew<Function>(constructor);
  Local<Object> instance = cons->NewInstance(argc, argv);

  return NanEscapeScope(instance);

}

NAN_METHOD(FrameBuffer::New) {
    NanScope();

    v8::String::Utf8Value path(args[0]->ToString());
    std::string _path = std::string(*path);

    FrameBuffer *obj = new FrameBuffer(_path.c_str());
    obj->drawToBuffer = args[1]->IsUndefined() ? false : args[1]->BooleanValue();

    obj->Wrap(args.This());
    NanReturnValue(args.This());
}

NAN_METHOD(FrameBuffer::Size) {
    NanScope();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    Local<Object> sizeObject = NanNew<Object>();

    sizeObject->Set(NanNew<String>("width"), NanNew<Number>(obj->vinfo.xres));
    sizeObject->Set(NanNew<String>("height"), NanNew<Number>(obj->vinfo.yres));

    NanReturnValue(sizeObject);
}

NAN_METHOD(FrameBuffer::Data) {
    NanScope();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    Local<Object> bufferObject = NanNewBufferHandle(obj->fbp, obj->screenSize);

    NanReturnValue(bufferObject);
}

NAN_METHOD(FrameBuffer::Clear) {
    NanScope();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    cairo_destroy(cr);

    NanReturnUndefined();
}

NAN_METHOD(FrameBuffer::Blit) {
    NanScope();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    if (obj->drawToBuffer) {
        cairo_t *cr = cairo_create (obj->screenSurface);
        cairo_set_source_surface (cr, obj->bufferSurface, 0, 0);
        cairo_paint (cr);

        cairo_destroy(cr);
    }

    NanReturnUndefined();
}

NAN_METHOD(FrameBuffer::Color) {
    NanScope();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    obj->r = (args[0]->NumberValue());
    obj->g = (args[1]->NumberValue());
    obj->b = (args[2]->NumberValue());

    NanReturnUndefined();
}

NAN_METHOD(FrameBuffer::Fill) {
    NanScope();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairo_set_source_rgb(cr, obj->r, obj->g, obj->b);
    cairo_paint(cr);

    cairo_destroy(cr);

    NanReturnUndefined();
}

NAN_METHOD(FrameBuffer::Line) {
    NanScope();

    double x0 = (args[0]->NumberValue());
    double y0 = (args[1]->NumberValue());
    double x1 = (args[2]->NumberValue());
    double y1 = (args[3]->NumberValue());

    double w = args[4]->IsUndefined() ? 1 : args[4]->NumberValue();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairo_set_source_rgb(cr, obj->r, obj->g, obj->b);

    cairo_move_to(cr, x0, y0);
    cairo_line_to(cr, x1, y1);

    cairo_set_line_width(cr, w);
    cairo_stroke(cr);

    cairo_destroy(cr);

    NanReturnUndefined();
}

NAN_METHOD(FrameBuffer::Rect) {
    NanScope();

    double x = (args[0]->NumberValue());
    double y = (args[1]->NumberValue());
    double w = (args[2]->NumberValue());
    double h = (args[3]->NumberValue());

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairo_set_source_rgb(cr, obj->r, obj->g, obj->b);

    cairo_rectangle(cr, x, y, w, h);

    if (!args[4]->IsUndefined() && args[4]->BooleanValue() == false) {
        double w = args[5]->IsUndefined() ? 1 : args[5]->NumberValue();
        cairo_set_line_width(cr, w);
        cairo_stroke(cr);
    } else if (!args[4]->IsUndefined() && args[4]->BooleanValue() == true) {
        cairo_fill(cr);
    } else {
        cairo_fill(cr);
    }

    cairo_destroy(cr);

    NanReturnUndefined();
}

NAN_METHOD(FrameBuffer::Circle) {
    NanScope();

    double x = (args[0]->NumberValue());
    double y = (args[1]->NumberValue());
    double radius = (args[2]->NumberValue());

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairo_set_source_rgb(cr, obj->r, obj->g, obj->b);

    cairo_arc(cr, x, y, radius, 0, 2*3.141592654);

    if (!args[3]->IsUndefined() && args[3]->BooleanValue() == false) {
        double w = args[4]->IsUndefined() ? 1 : args[4]->NumberValue();
        cairo_set_line_width(cr, w);
        cairo_stroke(cr);
    } else if (!args[3]->IsUndefined() && args[3]->BooleanValue() == true) {
        cairo_fill(cr);
    } else {
        cairo_fill(cr);
    }

    cairo_destroy(cr);

    NanReturnUndefined();
}

NAN_METHOD(FrameBuffer::Font) {
    NanScope();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    v8::String::Utf8Value fontName(args[0]->ToString());
    std::string _fontName = std::string(*fontName);

    obj->fontName = _fontName.c_str();
    obj->fontSize = args[1]->IsUndefined() ? 12 : args[1]->NumberValue();
    obj->fontBold = args[2]->IsUndefined() ? false : args[2]->BooleanValue();

    NanReturnUndefined();
}

NAN_METHOD(FrameBuffer::Text) {
    NanScope();

    double x = (args[0]->NumberValue());
    double y = (args[1]->NumberValue());

    v8::String::Utf8Value text(args[2]->ToString());
    std::string _text = std::string(*text);

    bool textCentered = args[3]->IsUndefined() ? false : args[3]->BooleanValue();
    double textRotation = args[4]->IsUndefined() ? 0 : args[4]->NumberValue();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairo_set_source_rgb(cr, obj->r, obj->g, obj->b);

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
    }

    cairo_show_text(cr, _text.c_str());

    cairo_destroy(cr);

    NanReturnUndefined();
}

NAN_METHOD(FrameBuffer::Image) {
    NanScope();

    double x = (args[0]->NumberValue());
    double y = (args[1]->NumberValue());

    v8::String::Utf8Value path(args[2]->ToString());
    std::string _path = std::string(*path);

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    cairo_t *cr = getDrawingContext(obj);

    cairo_surface_t *image = cairo_image_surface_create_from_png(_path.c_str());
    cairo_set_source_surface(cr, image, x, y);
    cairo_paint(cr);

    cairo_status_t status = cairo_status(cr);

    if (status != CAIRO_STATUS_SUCCESS) {
        NanThrowError("Error reading image");
    }

    cairo_surface_destroy(image);
    cairo_destroy(cr);

    NanReturnUndefined();
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
        NanThrowError("Error opening framebuffer device");
        return;
    }

    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        NanThrowError("Error retrieving data from framebuffer");
        return;
    }

    memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

    vinfo.bits_per_pixel = 8;
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)) {
        NanThrowError("Error sending data to framebuffer");
        return;
    }

    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        NanThrowError("Error retrieving data from framebuffer");
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
        NanThrowError("Error during memory mapping");
        return;
    }

    bufferSurface = cairo_image_surface_create_for_data ((unsigned char *)bbp, CAIRO_FORMAT_RGB16_565, vinfo.xres, vinfo.yres, cairo_format_stride_for_width(CAIRO_FORMAT_RGB16_565, vinfo.xres));

    if (cairo_surface_status(bufferSurface) != CAIRO_STATUS_SUCCESS) {
        NanThrowError("Error creating buffer surface");
        return;
    }

    screenSurface = cairo_image_surface_create_for_data ((unsigned char *)fbp, CAIRO_FORMAT_RGB16_565, vinfo.xres, vinfo.yres, cairo_format_stride_for_width(CAIRO_FORMAT_RGB16_565, vinfo.xres));

    if (cairo_surface_status(bufferSurface) != CAIRO_STATUS_SUCCESS) {
        NanThrowError("Error creating screeh surface");
    }
}

FrameBuffer::~FrameBuffer() {
    if ((int)fbp != -1) {
        free(bbp);
        munmap(fbp, screenSize);

        if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) {
            NanThrowError("Error restoring framebuffer state");
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
