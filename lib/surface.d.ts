import GObject = require('./object')
import GDKDisplay = require('./display')

/** What a window draws on, as a `GdkSurface`. */
interface GDKSurface extends GObject {
  readonly width: number

  readonly height: number

  readonly scaleFactor: number

  readonly display: GDKDisplay | null
}

declare class GDKSurface {
  protected constructor()
}

export = GDKSurface
