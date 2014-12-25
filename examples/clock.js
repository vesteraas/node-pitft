var pitft = require("../pitft");

var fb = new pitft.FrameBuffer("/dev/fb1");

fb.clear();

var rad = 180 / Math.PI;

var xMax = fb.size().width;
var yMax = fb.size().height;

fb.color(1, 1, 1);
fb.circle(xMax / 2, yMax / 2, 100);

fb.color(0, 0, 0);
for (var a=0; a<360; a+=30) {
    var x0 = xMax / 2 + Math.sin(a / rad) * 85;
    var y0 = yMax / 2 + Math.cos(a / rad) * 85;

    var x1 = xMax / 2 + Math.sin(a / rad) * 95;
    var y1 = yMax / 2 + Math.cos(a / rad) * 95;

    fb.line(x0, y0, x1, y1, 5);
}

fb.color(1, 0, 0);

var a, x0, y0, x1, y1;

a = 10 / 12 * 360;

x0 = xMax / 2 - Math.sin(a / rad);
y0 = yMax / 2 - Math.cos(a / rad);

x1 = xMax / 2 - Math.sin(a / rad) * 50;
y1 = yMax / 2 - Math.cos(a / rad) * 50;

fb.line(x0, y0, x1, y1, 5);

a = 2 / 12 * 360;

x0 = xMax / 2 - Math.sin(a / rad);
y0 = yMax / 2 - Math.cos(a / rad);

x1 = xMax / 2 - Math.sin(a / rad) * 80;
y1 = yMax / 2 - Math.cos(a / rad) * 80;

fb.line(x0, y0, x1, y1, 5);