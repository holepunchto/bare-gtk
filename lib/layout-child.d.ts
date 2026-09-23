import GObject = require('./object')
import GTKWidget = require('./widget')

/** What a layout manager keeps about one child, as a `GtkLayoutChild`. */
interface GTKLayoutChild extends GObject {
  readonly childWidget: GTKWidget | null
}

declare class GTKLayoutChild {
  protected constructor()
}

export = GTKLayoutChild
