var pitft = require("../pitft");

var fb = new pitft.FrameBuffer("/dev/fb1");

fb.clear();

var xMax = fb.size().width;
var yMax = fb.size().height;

for (var n=0; n<1000; n++) {
    var x, y, radius, r, g, b;

    x = parseInt(Math.random() * xMax, 10);
    y = parseInt(Math.random() * yMax, 10);
    radius = parseInt(Math.random() * yMax / 2, 10);

    r = Math.random();
    g = Math.random();
    b = Math.random();

    fb.color(r, g, b);
    fb.circle(x, y, radius, false, 5);
}
