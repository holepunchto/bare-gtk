import GObject = require('./object')
import GDKClipboard = require('./clipboard')
import GDKMonitor = require('./monitor')
import GDKSurface = require('./surface')

/** A connection to the display server, as a `GdkDisplay`. */
interface GDKDisplay extends GObject {
  /** The monitor that most of `surface` is on, or `null`. */
  getMonitorAtSurface(surface: GDKSurface): GDKMonitor | null

  /** The clipboard used for copy and paste. */
  getClipboard(): GDKClipboard

  /** The selection that middle click pastes. */
  getPrimaryClipboard(): GDKClipboard
}

declare class GDKDisplay {
  protected constructor()

  /** The display the app opened, or `null` before GTK is initialised. */
  static getDefault(): GDKDisplay | null
}

export = GDKDisplay
