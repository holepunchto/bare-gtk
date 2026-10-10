import GTKGestureSingle = require('./gesture-single')

/** Reports drags, as a `GtkGestureDrag`. */
interface GTKGestureDrag extends GTKGestureSingle<GTKGestureDrag.Events> {
  /** Where the drag started, or `null` while no drag is under way. */
  readonly startPoint: [x: number, y: number] | null

  /** How far the drag has moved, or `null` while no drag is under way. */
  readonly offset: [x: number, y: number] | null
}

declare class GTKGestureDrag {
  constructor()
}

declare namespace GTKGestureDrag {
  export interface Events {
    dragBegin: [startX: number, startY: number]
    dragUpdate: [offsetX: number, offsetY: number]
    dragEnd: [offsetX: number, offsetY: number]
    cancel: []
  }
}

export = GTKGestureDrag
