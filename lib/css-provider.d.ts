import GObject = require('./object')

/** A style sheet, as a `GtkCssProvider`. Apply it with `StyleContext.addProviderForDisplay()`. */
interface GTKCssProvider extends GObject {
  /** Load the style sheet from CSS source. */
  loadFromData(data: string): this
}

declare class GTKCssProvider {
  constructor()
}

export = GTKCssProvider
