import GObject = require('./object')

/** Anything a `GTKPicture` can show, as a `GdkPaintable`. A `GDKTexture` is one. */
interface GDKPaintable extends GObject {}

declare class GDKPaintable {
  protected constructor()
}

export = GDKPaintable
