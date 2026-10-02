import GTKLayoutManager = require('./layout-manager')
import BareFrameLayoutChild = require('./frame-layout-child')
import GTKWidget = require('./widget')

/**
 * A layout manager that gives each child an exact frame. GTK's own managers size a child from
 * what it prefers. Set a frame through the `BareFrameLayoutChild` of each child.
 */
interface BareFrameLayout extends GTKLayoutManager<BareFrameLayout.Events> {
  getLayoutChild(widget: GTKWidget): BareFrameLayoutChild | null
}

declare class BareFrameLayout {
  constructor()
}

declare namespace BareFrameLayout {
  export interface Events {
    /**
     * The widget is being laid out at `width` by `height`. Frames set from a listener apply in
     * this layout. A listener must not change the widget tree.
     */
    resize: [width: number, height: number]
  }
}

export = BareFrameLayout
