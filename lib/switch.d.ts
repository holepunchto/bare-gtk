import GTKWidget = require('./widget')

/** An on and off switch, as a `GtkSwitch`. */
interface GTKSwitch extends GTKWidget<GTKSwitch.Events> {
  active: boolean
}

declare class GTKSwitch {
  constructor()
}

declare namespace GTKSwitch {
  export interface Events {
    /** `active` changed, whether the user or the program changed it. */
    active: []
  }
}

export = GTKSwitch
