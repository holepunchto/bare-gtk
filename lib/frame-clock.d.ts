import GObject = require('./object')

/** What tells a toplevel when to draw its next frame, as a `GdkFrameClock`. */
interface GDKFrameClock extends GObject<GDKFrameClock.Events> {
  /** How many frames have been drawn. */
  readonly frameCounter: number

  /** When the current frame started, in microseconds on the monotonic clock. */
  readonly frameTime: number

  /** Draw a frame on every refresh until `endUpdating()`, whether or not anything changed. */
  beginUpdating(): this

  /** Undo one `beginUpdating()`. */
  endUpdating(): this
}

declare class GDKFrameClock {
  protected constructor()
}

declare namespace GDKFrameClock {
  export interface Events {
    /** A frame has been laid out and drawn. */
    afterPaint: []

    /** A frame is about to be laid out and drawn, so changes made now are part of it. */
    update: []
  }
}

export = GDKFrameClock
