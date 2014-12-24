var pitft = require("../pitft");

var fb = new pitft.FrameBuffer("/dev/fb1");

fb.clear();

fb.color(1, 1, 1);
fb.font("Courier", 24);
fb.text(50, 50, "Hei gutt! ÆØÅ");
