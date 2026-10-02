import GTKWidget = require('./widget')
import GDKSurface = require('./surface')

/** A top level window, as a `GtkWindow`. */
interface GTKWindow extends GTKWidget<GTKWindow.Events> {
  /** The surface the window draws on, or `null` before it is shown. */
  readonly surface: GDKSurface | null

  title: string | null

  /** The size the window opens at, as `[width, height]`. */
  get defaultSize(): [width: number, height: number]
  set defaultSize(size: [width: number, height: number])

  /** The one widget the window holds, or `null`. */
  child: GTKWidget | null
}

declare class GTKWindow {
  /** Create a hidden window. Set `visible` to show it. */
  constructor()
}

declare namespace GTKWindow {
  export interface Events {
    /** The user asked to close the window. */
    'close-request': []
  }
}

export = GTKWindow
