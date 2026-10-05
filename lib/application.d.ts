import GObject = require('./object')

/** The application, as a `GtkApplication`. It quits once it has no windows and nothing holds it. */
interface GTKApplication extends GObject {
  /** Keep the application running while it has no windows. */
  hold(): this

  /** Undo one `hold()`. */
  release(): this
}

declare class GTKApplication {
  protected constructor()

  /** The application the runtime started, or `null` if there is none. */
  static getDefault(): GTKApplication | null
}

export = GTKApplication
