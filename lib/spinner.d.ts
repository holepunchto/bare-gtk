import GTKWidget = require('./widget')

/** A spinning busy indicator, as a `GtkSpinner`. */
interface GTKSpinner extends GTKWidget {
  spinning: boolean
}

declare class GTKSpinner {
  constructor()
}

export = GTKSpinner
