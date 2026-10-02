import GDKPaintable = require('./paintable')

/** An image in memory, as a `GdkTexture`. */
interface GDKTexture extends GDKPaintable {
  readonly width: number

  readonly height: number
}

declare class GDKTexture {
  protected constructor()

  /**
   * Load an image file.
   * @throws When the file cannot be read or is not an image.
   */
  static newFromFilename(path: string): GDKTexture
}

export = GDKTexture
