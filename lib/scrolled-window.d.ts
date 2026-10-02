import GTKWidget = require('./widget')
import GTKAdjustment = require('./adjustment')

/** A widget that scrolls its child, as a `GtkScrolledWindow`. */
interface GTKScrolledWindow extends GTKWidget {
  child: GTKWidget | null

  /** What horizontal scrolling moves. Read and set its `value` to scroll. */
  readonly hadjustment: GTKAdjustment

  /** What vertical scrolling moves. Read and set its `value` to scroll. */
  readonly vadjustment: GTKAdjustment

  /** When each scroll bar is shown, as `POLICY_*` constants. */
  setPolicy(horizontal: number, vertical: number): this
}

declare class GTKScrolledWindow {
  constructor()
}

export = GTKScrolledWindow
