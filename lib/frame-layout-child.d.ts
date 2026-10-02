import GTKLayoutChild = require('./layout-child')

/** The frame of one child of a `BareFrameLayout`. */
interface BareFrameLayoutChild extends GTKLayoutChild {
  /** Where the child is placed, as `[x, y, width, height]`. */
  get frame(): [x: number, y: number, width: number, height: number]
  set frame(frame: [x: number, y: number, width: number, height: number])

  /** A 3D transform for the child, as sixteen numbers in the order CSS uses for `matrix3d()`. */
  set transform(transform: number[])
}

declare class BareFrameLayoutChild {
  protected constructor()
}

export = BareFrameLayoutChild
