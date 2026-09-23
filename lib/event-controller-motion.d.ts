import GTKEventController = require('./event-controller')

/** Reports pointer movement over a widget, as a `GtkEventControllerMotion`. */
interface GTKEventControllerMotion extends GTKEventController<GTKEventControllerMotion.Events> {}

declare class GTKEventControllerMotion {
  constructor()
}

declare namespace GTKEventControllerMotion {
  export interface Events {
    motion: [x: number, y: number]
    enter: [x: number, y: number]
    leave: []
  }
}

export = GTKEventControllerMotion
