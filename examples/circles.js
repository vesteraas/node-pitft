var pitft = require("../pitft");

var fb = new pitft.FrameBuffer("/dev/fb1");

fb.clear();

for (var n = 0; n < 1000; n++) {
    var x, y, radius, r, g, b;

    x = parseInt(Math.random() * 320, 10);
    y = parseInt(Math.random() * 240, 10);
    radius = parseInt(Math.random() * 120, 10);

    r = parseInt(Math.random() * 255, 10);
    g = parseInt(Math.random() * 255, 10);
    b = parseInt(Math.random() * 255, 10);

    fb.circle(x, y, radius, r, g, b);
}