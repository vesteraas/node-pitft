var pitft = require("../pitft");

var fb = pitft("/dev/fb1"); // Returns a framebuffer in direct mode.  See the clock.js example for double buffering mode

// Clear the screen buffer
fb.clear();

var xMax = fb.size().width;
var yMax = fb.size().height;

// Fill the background with a gradient from black to white
var pattern0 = fb.patternCreateLinear(0, 0, xMax, yMax); // xStart, yStart, xEnd, yEnd
fb.patternAddColorStop(pattern0, 0, 0, 0, 0); // patternID, offset, r, g, b
fb.patternAddColorStop(pattern0, 1, 1, 1, 1);
fb.color(pattern0);
fb.fill();

// Destroy a pattern, if it's not used anymore to free up memory
fb.patternDestroy(pattern0);

// Create two gradient pattens
var pattern1 = fb.patternCreateLinear(0, 0, xMax, 0);
fb.patternAddColorStop(pattern1, 0, 1, 0, 0);
fb.patternAddColorStop(pattern1, 0.5, 1, 1, 0);
fb.patternAddColorStop(pattern1, 1, 0, 1, 0);

var pattern2 = fb.patternCreateLinear(0, 0, xMax, 0);
fb.patternAddColorStop(pattern2, 0, 0, 0, 1);
fb.patternAddColorStop(pattern2, 1, 0, 1, 0);

// Draw a bar at the top using pattern1
fb.color(pattern1);
fb.rect(0, 0, xMax, 20);

// Draw some circles using pattern1 and pattern2
fb.circle(xMax/2, yMax/2, 60, false, 10);

fb.color(pattern2);
fb.circle(xMax/2, yMax/2, 40, false, 10);

fb.color(pattern1);
fb.circle(xMax/2, yMax/2, 20);

// Write some text
fb.font("fantasy", 24);
fb.text(xMax/2, 30, "Some text", true);

// Draw a green fade-in bar at the bottom, reuse and override pattern1
pattern1 = fb.patternCreateLinear(pattern1 ,0, 0, xMax, 0); // patternID, xStart, yStart, xEnd, yEnd
fb.patternAddColorStop(pattern1, 0, 0, 1, 0, 0); // patternID, offset, r, g, b, a
fb.patternAddColorStop(pattern1, 1, 0, 1, 0, 1);
fb.color(pattern1);
fb.rect(0, yMax-20, xMax, 40);

// Create a RGBA pattern and draw a line across the screen
var pattern4 = fb.patternCreateRGB(1, 0, 1, 0.5); // r, g, b, a
fb.color(pattern4);
fb.line(0, 0, xMax, yMax, 10);

// Reuse pattern4
pattern4 = fb.patternCreateRGB(pattern4, 0, 1, 1, 0.3); // patternID, r, g, b, a
fb.color(pattern4);
fb.line(xMax, 0, 0, yMax, 10);
