import GObject = require('./object')
import GTKWindow = require('./window')

/** The application, as a `GtkApplication`. It quits once it has no windows and nothing holds it. */
interface GTKApplication extends GObject {
  /**
   * The window that was most recently active, or `null` if there are none. Until a window has been
   * focused, this is the window added most recently.
   */
  readonly activeWindow: GTKWindow | null

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
