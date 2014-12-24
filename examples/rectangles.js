var pitft = require("../pitft");

var fb = new pitft.FrameBuffer("/dev/fb1");

fb.clear();

var xMax = fb.size().width;
var yMax = fb.size().height;

for (var n=0; n<1000; n++) {
    var x, y, w, h, r, g, b;

    do {
        x = parseInt(Math.random() * xMax, 10);
        w = parseInt(Math.random() * xMax, 10);
    } while ((x + w) >= xMax)

    do {
        y = parseInt(Math.random() * yMax, 10);
        h = parseInt(Math.random() * yMax, 10);
    } while ((y + h) >= yMax)

    r = Math.random();
    g = Math.random();
    b = Math.random();

    fb.color(r, g, b);
    fb.rect(x, y, w, h, true);
}