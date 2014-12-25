var pitft = require("../pitft");

var fb = new pitft.FrameBuffer("/dev/fb1", true);
fb.clear();

var xMax = fb.size().width;
var yMax = fb.size().height;
var rad = 180/Math.PI;

var hand = function(_fb, x, y, angle, length, width) {
    var x0 = xMax/2 + Math.sin(angle/rad);
    var y0 = yMax/2 - Math.cos(angle/rad);

    var x1 = xMax/2 + Math.sin(angle/rad) * length;
    var y1 = yMax/2 - Math.cos(angle/rad) * length;

    fb.line(x0, y0, x1, y1, width);
}

var drawDial = function() {
    fb.color(1, 1, 1);
    fb.circle(xMax/2, yMax/2, 100);

    fb.color(0, 0, 0);
    for (var a = 0; a < 360; a += 6) {
        var x0, y0;

        var x0 = xMax/2 + Math.sin(a/rad) * 95;
        var y0 = yMax/2 + Math.cos(a/rad) * 95;

        if (a % 30 == 0) {
            x1 = xMax/2 + Math.sin(a/rad) * 85;
            y1 = yMax/2 + Math.cos(a/rad) * 85;
            fb.line(x0, y0, x1, y1, 5);
        } else {
            x1 = xMax/2 + Math.sin(a/rad) * 90;
            y1 = yMax/2 + Math.cos(a/rad) * 90;
            fb.line(x0, y0, x1, y1, 1);
        }
    }
}

var updateHands = function() {
    fb.color(1, 1, 1);
    fb.circle(xMax/2, yMax/2, 85);

    var now = new Date(),
        midnight = new Date(
            now.getFullYear(),
            now.getMonth(),
            now.getDate(),
            0,0,0),
        hours = (now.getTime() - midnight.getTime())/1000/60/60,
        minutes = (hours * 60) % 60;
        seconds = parseInt((minutes * 60) % 60);

    fb.color(1, 0, 0);
    hand(fb, 0, 0, hours/12 * 360, 60, 5);
    hand(fb, 0, 0, minutes/60 * 360, 80, 5);
    fb.color(0, 0, 0);
    hand(fb, 0, 0, seconds/60 * 360, 80, 1.5);
    fb.color(1, 0, 0);
    fb.circle(xMax/2, yMax/2, 7.5);
    fb.blit();
};

drawDial();
setInterval(function() {
    updateHands();
}, 100);