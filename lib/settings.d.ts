import GObject = require('./object')

/** The settings of the desktop, as a `GtkSettings`. */
interface GTKSettings extends GObject<GTKSettings.Events> {
  /** Whether the dark variant of the theme is used. */
  preferDarkTheme: boolean

  /** The font resolution, in 1024ths of a dot per inch. -1 means the default. */
  xftDpi: number
}

declare class GTKSettings {
  protected constructor()

  static getDefault(): GTKSettings | null
}

declare namespace GTKSettings {
  export interface Events {
    preferDarkTheme: []
    xftDpi: []
  }
}

export = GTKSettings
