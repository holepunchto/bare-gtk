import EventEmitter from 'bare-events'
import { tag, handle, Handle } from 'bare-gobject-registry'

/**
 * The base of every GTK and GDK object in this module. Objects that have events emit them as
 * ordinary events, and GTK is only asked to report an event while something listens to it.
 */
interface GObject<M extends Record<keyof M, unknown[]> = {}> extends EventEmitter<M> {
  readonly [tag]: number

  readonly [handle]: Handle
}

declare class GObject<M extends Record<keyof M, unknown[]> = {}> {
  protected constructor()
}

export = GObject
