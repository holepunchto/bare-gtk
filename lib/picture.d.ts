import { Wrapper } from 'bare-gobject-registry'
import GTKWidget = require('./widget')
import GDKPaintable = require('./paintable')

/** A widget that shows an image, as a `GtkPicture`. */
interface GTKPicture extends GTKWidget {
  /** What the picture shows, or `null`. It can come from another addon. */
  get paintable(): GDKPaintable | null
  set paintable(paintable: Wrapper | null)

  /** Whether the image keeps its aspect ratio when it is scaled. */
  keepAspectRatio: boolean

  /** Whether the picture can be made smaller than the image. */
  canShrink: boolean
}

declare class GTKPicture {
  constructor()
}

export = GTKPicture
