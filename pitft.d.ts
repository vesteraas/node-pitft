// Type definitions for pitft
// Definitions by: Peter Müller <https://crycode.de/>

/// <reference types="node" />

declare module "pitft" {

  /**
   * Creates a new PiTFT instance.
   * @param  {string}            device          The framebuffer device. (e.g. /dev/fb1)
   * @param  {boolean}           doubleBuffering True if you want to use double buffering.
   */
  function pitft (device: string, doubleBuffering?: boolean): pitft.FrameBuffer;

  namespace pitft {
    interface FrameBuffer {

      /**
       * Returns an object with the size of the display.
       */
      size (): {
        /**
         * Width of the display.
         */
        width: number;

        /**
         * Height of the display.
         */
        height: number;
      };

      /**
       * Returns the raw data as a Buffer.
       */
      data (): Buffer;

      /**
       * Clears the display.
       */
      clear (): void;

      /**
       * Transfers the current Buffer to the display.
       * Must be called in double buffering mode.
       */
      blit (): void;

      /**
       * Selects a pattern for the next drawings.
       * @param {number} patternID ID of the pattern.
       */
      color (patternID: number): void;

      /**
       * Selects a color for the nex drawings.
       * @param {number} r Red (from 0.0 to 1.0)
       * @param {number} g Green (from 0.0 to 1.0)
       * @param {number} b Blue (from 0.0 to 1.0)
       */
      color (r: number, g: number, b: number): void;

      /**
       * Creates a new linear pattern and returns the pattern ID.
       * Start and end coordinates are absolute to the display size.
       * @param  {number} x0 Start x of the linear pattern.
       * @param  {number} y0 Start y of the linear pattern.
       * @param  {number} x1 End x of the linear pattern.
       * @param  {number} y2 End y of the linear pattern.
       * @return {number}    The ID of the new pattern.
       */
      patternCreateLinear (x0: number, y0: number, x1: number, y2: number) : number;

      /**
       * Creates a new linear pattern and returns the pattern ID.
       * Start and end coordinates are absolute to the display size.
       * @param  {number} patternID ID, which the pattern will get. Old patterns with this ID will be replaced.
       * @param  {number} x0        Start x of the linear pattern.
       * @param  {number} y0        Start y of the linear pattern.
       * @param  {number} x1        End x of the linear pattern.
       * @param  {number} y2        End y of the linear pattern.
       * @return {number}           The ID of the new pattern.
       */
      patternCreateLinear (patternID: number, x0: number, y0: number, x1: number, y2: number) : number;

      /**
       * Creates a new RGBA pattern and returns the pattern ID.
       * @param  {number} r Red (from 0.0 to 1.0)
       * @param  {number} g Green (from 0.0 to 1.0)
       * @param  {number} b Blue (from 0.0 to 1.0)
       * @param  {number} a (optional) Alpha from 0.0 to 1.0)
       * @return {number}   The ID of the new pattern.
       */
      patternCreateRGB (r: number, g: number, b: number, a?: number): number;

      /**
       * Creates a new RGBA pattern and returns the pattern ID.
       * @param  {number} patternID ID, which the pattern will get. Old patterns with this ID will be replaced.
       * @param  {number} r         Red (from 0.0 to 1.0)
       * @param  {number} g         Green (from 0.0 to 1.0)
       * @param  {number} b         Blue (from 0.0 to 1.0)
       * @param  {number} a         (optional) Alpha (from 0.0 to 1.0)
       * @return {number}           The ID of the new pattern.
       */
      patternCreateRGB (patternID: number, r: number, g: number, b: number, a?: number): number;

      /**
       * Adds a color stop to a linear pattern.
       * @param {number} patternID ID of the pattern.
       * @param {number} offset    Offset in the linear pattern. (from 0.0 to 1.0)
       * @param {number} r         Red (from 0.0 to 1.0)
       * @param {number} g         Green (from 0.0 to 1.0)
       * @param {number} b         Blue (from 0.0 to 1.0)
       * @param {number} a         (optional) Alpha (from 0.0 to 1.0)
       */
      patternAddColorStop (patternID: number, offset: number, r: number, g: number, b: number, a?: number): void;

      /**
       * Destroyes a pattern to free up memory.
       * A destroyed pattern cannot be used anymore.
       * @param {number} patternID [description]
       */
      patternDestroy (patternID: number): void;

      /**
       * Fills the display with the currently selected color or pattern.
       */
      fill (): void;

      /**
       * Creates a line using the currently selected color or pattern.
       * @param {number} x0    Start x
       * @param {number} y0    Start y
       * @param {number} x1    End x
       * @param {number} y1    End y
       * @param {number} width (optional) Line width.
       */
      line (x0: number, y0: number, x1: number, y1: number, width?: number): void;

      /**
       * Creates a rectangel using the currently selected color or pattern.
       * @param {number}  x           Start x
       * @param {number}  y           Start y
       * @param {number}  width       Width
       * @param {number}  height      Height
       * @param {boolean} outline     (optional) true to draw only the border.
       * @param {number}  borderWidth (optional) Width of the border if outline is true.
       */
      rect (x: number, y: number, width: number, height: number, outline?: boolean, borderWidth?: number): void;

      /**
       * Creates a circle using the currently selected color or pattern.
       * @param {number}  x           Center x
       * @param {number}  y           Center y
       * @param {number}  radius      Radius
       * @param {boolean} outline     (optional) true to draw only the border.
       * @param {boolean} borderWidth (optional) Width of the border if outline is true.
       */
      circle (x: number, y: number, radius: number, outline?: boolean, borderWidth?: boolean): void;

      /**
       * Selects a font.
       * @param {string}  fontName The font name.
       * @param {number}  fontSize The font size.
       * @param {boolean} fontBold (optional) true to draw the font bold.
       */
      font (fontName: string, fontSize: number, fontBold?: boolean): void;

      /**
       * Creates a text using the currently selected color or pattern.
       * @param {number}  x        Start x
       * @param {number}  y        Start y (baseline)
       * @param {string}  text     The text
       * @param {boolean} centered (optional) true to draw the text centered to the start coordinates.
       * @param {number}  rotation (optional) Rotation of the text in degrees.
       * @param {boolean} right    (optional) true to draw the text rigth aligned to the start coordinates.
       */
      text (x: number, y: number, text: string, centered?: boolean, rotation?: number, right?: boolean): void;

      /**
       * Draws an image.
       * @param {number} x    Start x
       * @param {number} y    Start y
       * @param {string} path Path to the image file.
       */
      image (x: number, y: number, path: string): void;
    }
  }

  export = pitft;
}
