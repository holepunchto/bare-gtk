import GTKEventController = require('./event-controller')

/** Reports when a widget gets and loses the keyboard focus, as a `GtkEventControllerFocus`. */
interface GTKEventControllerFocus extends GTKEventController<GTKEventControllerFocus.Events> {}

declare class GTKEventControllerFocus {
  constructor()
}

declare namespace GTKEventControllerFocus {
  export interface Events {
    enter: []
    leave: []
  }
}

export = GTKEventControllerFocus
