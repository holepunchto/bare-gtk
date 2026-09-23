import GObject = require('./object')
import GTKDevice = require('./device')

/** The base of the objects that report input on a widget, as a `GtkEventController`. */
interface GTKEventController<M extends Record<keyof M, unknown[]> = {}> extends GObject<M> {
  /** When the controller sees an event, as a `PROPAGATION_PHASE_*` constant. */
  propagationPhase: number

  /** The device of the event being handled, or `null` outside an event. */
  readonly currentEventDevice: GTKDevice | null
}

declare class GTKEventController<M extends Record<keyof M, unknown[]> = {}> {
  protected constructor()
}

export = GTKEventController
