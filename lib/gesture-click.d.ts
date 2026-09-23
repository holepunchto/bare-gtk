import GTKGestureSingle = require('./gesture-single')

/** Reports clicks and taps, as a `GtkGestureClick`. */
interface GTKGestureClick extends GTKGestureSingle<GTKGestureClick.Events> {}

declare class GTKGestureClick {
  constructor()
}

declare namespace GTKGestureClick {
  export interface Events {
    /** `presses` counts quick repeated clicks, so 2 is a double click. */
    pressed: [presses: number, x: number, y: number]
    released: [presses: number, x: number, y: number]
    cancel: []
  }
}

export = GTKGestureClick
