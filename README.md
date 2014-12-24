pitft
=====

### A [NodeJS](http://nodejs.org) module for the Adafruit PiTFT family of displays for the [Raspberry Pi](http://www.raspberrypi.org) computer.

This module is only tested on the [2.8" PiTFT](http://www.adafruit.com/product/1601) but should work on other displays from Adafruit.

## Author
  - Werner Vesterås <wvesteraas@gmail.com>

## Installation

Prior to installing this module, you must install the [FBTFT drivers by notro](https://github.com/notro/fbtft).  See the [wiki](https://github.com/notro/fbtft/wiki) page for more details.

You will also need to install the [Cairo](http://cairographics.org) library.  This can be done from the console on your Raspberry Pi:

    sudo apt-get install libcairo2-dev

Finally, you can install the pitft module itself:

    npm install pitft

## Examples

Examples of usage can be found in the [examples](https://github.com/vesteraas/node-pitft/tree/master/examples) directory.