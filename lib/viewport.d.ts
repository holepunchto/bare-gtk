import GTKWidget = require('./widget')

/** Lets a widget that cannot scroll by itself be put in a `GTKScrolledWindow`, as a `GtkViewport`. */
interface GTKViewport extends GTKWidget {
  child: GTKWidget | null

  /**
   * Whether the child gets its minimum width or the width it asks for, as a `SCROLL_*` constant.
   */
  hscrollPolicy: number

  /**
   * Whether the child gets its minimum height or the height it asks for, as a `SCROLL_*` constant.
   */
  vscrollPolicy: number
}

declare class GTKViewport {
  constructor()
}

export = GTKViewport
