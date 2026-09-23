import GObject = require('./object')
import GTKLayoutChild = require('./layout-child')
import GTKWidget = require('./widget')

/** Places the children of a widget, as a `GtkLayoutManager`. */
interface GTKLayoutManager<M extends Record<keyof M, unknown[]> = {}> extends GObject<M> {
  /** What the manager keeps about `widget`, which must be a child of its widget. */
  getLayoutChild(widget: GTKWidget): GTKLayoutChild | null
}

declare class GTKLayoutManager<M extends Record<keyof M, unknown[]> = {}> {
  protected constructor()
}

export = GTKLayoutManager
