import GObject = require('./object')
import GDKDisplay = require('./display')

/** What a window draws on, as a `GdkSurface`. */
interface GDKSurface extends GObject {
  readonly width: number

  readonly height: number

  readonly scaleFactor: number

  readonly display: GDKDisplay | null

  /** The X11 window of the surface, as `gdk_x11_surface_get_xid()`, or `null` if GDK is not on X11. */
  readonly xid: number | null
}

declare class GDKSurface {
  protected constructor()
}

export = GDKSurface
