import GTKFixed = require('./fixed')

/** A `GTKFixed` that places each child at an exact frame, using a `BareFrameLayout`. */
interface BareFrameFixed extends GTKFixed {}

declare class BareFrameFixed {
  constructor()
}

export = BareFrameFixed
