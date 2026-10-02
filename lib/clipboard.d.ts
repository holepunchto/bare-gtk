import GObject = require('./object')

/** A clipboard, as a `GdkClipboard`. Get one from a `GDKDisplay`. */
interface GDKClipboard extends GObject {
  /**
   * Read the text on the clipboard. `callback` is called with an error message, or with `null`
   * and the text, which is `null` if there is none.
   */
  readTextAsync(callback: (error: string | null, text: string | null) => void): void

  /** Put `text` on the clipboard. */
  setText(text: string): this

  /** The mime types of what the clipboard holds. */
  readonly formats: string[]
}

declare class GDKClipboard {
  protected constructor()
}

export = GDKClipboard
