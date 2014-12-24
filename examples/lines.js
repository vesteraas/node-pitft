var pitft = require("../pitft");

var fb = new pitft.FrameBuffer("/dev/fb1");

fb.clear();

var xMax = fb.size().width;
var yMax = fb.size().height;

for (var n=0; n<10000; n++) {
    var x0, y0, x1, y1, r, g, b;

    x0 = parseInt(Math.random() * xMax, 10);
    y0 = parseInt(Math.random() * yMax, 10);

    x1 = parseInt(Math.random() * xMax, 10);
    y1 = parseInt(Math.random() * yMax, 10);

    r = parseInt(Math.random() * 255, 10);
    g = parseInt(Math.random() * 255, 10);
    b = parseInt(Math.random() * 255, 10);

    fb.line(x0, y0, x1, y1, r, g, b);
}