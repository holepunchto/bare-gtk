import GTKEventController = require('./event-controller')

/** The base of the controllers that recognise gestures, as a `GtkGesture`. */
interface GTKGesture<M extends Record<keyof M, unknown[]> = {}> extends GTKEventController<M> {
  /** Claim or deny the current event sequence, as an `EVENT_SEQUENCE_STATE_*` constant. */
  setState(state: number): boolean
}

declare class GTKGesture<M extends Record<keyof M, unknown[]> = {}> {
  protected constructor()
}

export = GTKGesture
