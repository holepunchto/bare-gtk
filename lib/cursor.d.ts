import GObject = require('./object')
import GDKTexture = require('./texture')

/** A mouse cursor, as a `GdkCursor`. */
interface GDKCursor extends GObject {
  readonly name: string | null

  readonly fallback: GDKCursor | null

  readonly texture: GDKTexture | null

  readonly hotspotX: number

  readonly hotspotY: number
}

declare class GDKCursor {
  protected constructor()

  /**
   * Create the cursor called `name`, such as `'pointer'`. `fallback` is shown where that cursor
   * does not exist.
   */
  static newFromName(name: string, fallback?: GDKCursor | null): GDKCursor | null

  /** Create a cursor from a texture, with its hotspot at `hotspotX`, `hotspotY`. */
  static newFromTexture(
    texture: GDKTexture,
    hotspotX: number,
    hotspotY: number,
    fallback?: GDKCursor | null
  ): GDKCursor | null
}

export = GDKCursor
