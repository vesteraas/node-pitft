var pitft = require("../pitft");

var fb = pitft("/dev/fb1");
fb.clear();

var xMax = fb.size().width;
var yMax = fb.size().height;

fb.color(1, 1, 1);

for (var a=0; a<=90; a+=15) {
    fb.font("fantasy", 12, false, a);
    fb.text(20, 20, "Rotated text");
}

for (var a=0; a<=180; a+=15) {
    fb.font("fantasy", 24, true, a);
    fb.text(xMax/2, yMax/2, "Rotated text");
}

for (var a=180; a<=270; a+=15) {
    fb.font("fantasy", 12, false, a);
    fb.text(xMax-20, yMax-20, "Rotated text");
}
