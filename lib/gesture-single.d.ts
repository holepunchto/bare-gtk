import GTKGesture = require('./gesture')

/** A gesture made with one touch or one mouse button, as a `GtkGestureSingle`. */
interface GTKGestureSingle<M extends Record<keyof M, unknown[]> = {}> extends GTKGesture<M> {
  /** The mouse button the gesture listens to. 0 means any button. */
  button: number
}

declare class GTKGestureSingle<M extends Record<keyof M, unknown[]> = {}> {
  protected constructor()
}

export = GTKGestureSingle
