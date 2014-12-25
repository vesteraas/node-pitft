var pitft = require("../pitft");

var fb = new pitft.FrameBuffer("/dev/fb1");
fb.clear();

var xMax = fb.size().width;
var yMax = fb.size().height;

fb.image(x, y, "raspberry-pi.png");

for (var n=0; n<1000; n++) {
    var x = Math.random() * (xMax + 32) - 16;
    var y = Math.random() * (yMax + 32) - 16;

    fb.image(x, y, "raspberry-pi-icon.png");
}
