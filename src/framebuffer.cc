#include "framebuffer.h"

using namespace v8;

Persistent<Function> FrameBuffer::constructor;

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

    screensize = finfo.smem_len;
    fbp = (char*)mmap(0, 
                      screensize, 
                      PROT_READ | PROT_WRITE, 
                      MAP_SHARED, 
                      fbfd, 
                      0);

    if ((int)fbp == -1) {
        NanThrowError("Error during memory mapping");
        return;
    }

    surface = cairo_image_surface_create_for_data ((unsigned char *)fbp, CAIRO_FORMAT_RGB16_565, vinfo.xres, vinfo.yres, cairo_format_stride_for_width(CAIRO_FORMAT_RGB16_565, vinfo.xres));
}

FrameBuffer::~FrameBuffer() {
    if ((int)fbp != -1) {
        munmap(fbp, screensize);

        if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) {
            NanThrowError("Error restoring framebuffer state");
        }
    }

    if (fbfd != -1) {
        close(fbfd);
    }

    if (surface) {
        cairo_surface_destroy(surface);
    }
}

void FrameBuffer::Init(Handle<Object> exports) {
    NanScope();

    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew("FrameBuffer"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);
    NODE_SET_PROTOTYPE_METHOD(tpl, "data", Data);
    NODE_SET_PROTOTYPE_METHOD(tpl, "clear", Clear);
    NODE_SET_PROTOTYPE_METHOD(tpl, "color", Color);
    NODE_SET_PROTOTYPE_METHOD(tpl, "fill", Fill);
    NODE_SET_PROTOTYPE_METHOD(tpl, "line", Line);
    NODE_SET_PROTOTYPE_METHOD(tpl, "rect", Rect);
    NODE_SET_PROTOTYPE_METHOD(tpl, "circle", Circle);

    NanAssignPersistent(constructor, tpl->GetFunction());
    exports->Set(NanNew("FrameBuffer"), tpl->GetFunction());
}

NAN_METHOD(FrameBuffer::New) {
    NanScope();

    if (args.IsConstructCall()) {
        v8::String::Utf8Value path(args[0]->ToString());
        std::string _path = std::string(*path);
        FrameBuffer *obj = new FrameBuffer(_path.c_str());
        obj->Wrap(args.This());
        NanReturnValue(args.This());
    } else {
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = NanNew<Function>(constructor);
        NanReturnValue(cons->NewInstance(argc, argv));
    }
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

    Local<Object> bufferObject = NanNewBufferHandle(obj->fbp, obj->screensize);

    NanReturnValue(bufferObject);
}

NAN_METHOD(FrameBuffer::Clear) {
    NanScope();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    memset(obj->fbp, 0x00, obj->screensize);

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

    cairo_t *cr = cairo_create (obj->surface);

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

    int w = args[4]->IsUndefined() ? 1 : (int)args[4]->NumberValue();

    FrameBuffer *obj = ObjectWrap::Unwrap<FrameBuffer>(args.Holder());

    cairo_t *cr = cairo_create(obj->surface);

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

    cairo_t *cr = cairo_create(obj->surface);

    cairo_set_source_rgb(cr, obj->r, obj->g, obj->b);

    cairo_rectangle(cr, x, y, w, h);

    if (!args[4]->IsUndefined() && args[4]->BooleanValue() == false) {
        int w = args[5]->IsUndefined() ? 1 : (int)args[5]->NumberValue();
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

    cairo_t *cr = cairo_create(obj->surface);

    cairo_set_source_rgb(cr, obj->r, obj->g, obj->b);

    cairo_arc(cr, x, y, radius, 0, 2*3.141592654);

    if (!args[3]->IsUndefined() && args[3]->BooleanValue() == false) {
        int w = args[4]->IsUndefined() ? 1 : (int)args[4]->NumberValue();
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
