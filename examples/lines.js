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

    r = Math.random();
    g = Math.random();
    b = Math.random();

    fb.color(r, g, b);
    fb.line(x0, y0, x1, y1, 1, r, g, b);
}
