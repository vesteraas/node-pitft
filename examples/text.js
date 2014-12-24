var pitft = require("../pitft");

var fb = new pitft.FrameBuffer("/dev/fb1");

fb.clear();

var yMax = fb.size().height;

fb.color(1, 1, 1);

var fontSize = 12;
var rotation = 0;
for (var y=0; y<yMax; y+=18) {
    fb.font("fantasy", fontSize, rotation);
    fb.text(0, y + 12, "This text is rotated " + rotation + " degrees.");
    fontSize += 2;
    rotation += 2;
}