var pitft = require("../pitft");

var fb = new pitft.FrameBuffer("/dev/fb1");

fb.clear();

for (var n=0; n<1000; n++) {
    var x, y, w, h, r, g, b;

    do {
        x = parseInt(Math.random() * 320, 10);
        w = parseInt(Math.random() * 320, 10);
    } while ((x + w) >= 320)

    do {
        y = parseInt(Math.random() * 240, 10);
        h = parseInt(Math.random() * 240, 10);
    } while ((y + h) >= 240)

    r = parseInt(Math.random() * 255, 10);
    g = parseInt(Math.random() * 255, 10);
    b = parseInt(Math.random() * 255, 10);

    fb.rect(x, y, w, h, r, g, b);
}