#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <cairo/cairo.h>

#include <nan.h>

class FrameBuffer : public node::ObjectWrap {
    public:
        static void Init(v8::Handle<v8::Object> exports);
    
    private:
        FrameBuffer(const char *path);        
        ~FrameBuffer();

        static NAN_METHOD(New);
        static NAN_METHOD(Size);
        static NAN_METHOD(Data);
        static NAN_METHOD(Clear);
        static NAN_METHOD(Color);
        static NAN_METHOD(Fill);
        static NAN_METHOD(Line);
        static NAN_METHOD(Rect);
        static NAN_METHOD(Circle);
        static v8::Persistent<v8::Function> constructor;

        int fbfd;
        struct fb_var_screeninfo orig_vinfo;
        struct fb_var_screeninfo vinfo;
        struct fb_fix_screeninfo finfo;
        long int screensize;
        char *fbp;

        cairo_surface_t *surface;

        double r, g, b;
};

#endif
