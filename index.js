var addon = require('bindings')('addon');

var obj = new addon.FrameBuffer("/dev/fb1");
obj.clear();

/*
for (var n=0; n<10000; n++) {
     var x, y, w, h, r, g, b;

     do {
          x = parseInt(Math.random() * 320, 10);
          w = parseInt(Math.random() * 320, 10);
     } while ((x + w) >= 320)

     do {
          y = parseInt(Math.random() * 240, 10);
          h = parseInt(Math.random() * 240, 10);
     } while ((y + h) >= 240)

     r = parseInt(Math.random() * 255, 10);
     g = parseInt(Math.random() * 255, 10);
     b = parseInt(Math.random() * 255, 10);

     obj.rect(x, y, w, h, r, g, b);

     if (n % 100 === 0) {
          console.log(n);
     }
}
*/

console.log(obj.dimension());

/*
var drawCircle = function() {
     var x, y, radius, r, g, b;

     x = parseInt(Math.random() * 320, 10);
     y = parseInt(Math.random() * 240, 10);
     radius = parseInt(Math.random() * 120, 10);

     r = parseInt(Math.random() * 255, 10);
     g = parseInt(Math.random() * 255, 10);
     b = parseInt(Math.random() * 255, 10);

     obj.circle(x, y, radius, r, g, b);
}

setInterval(drawCircle, 20);
*/