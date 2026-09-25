const binding = require('../binding')
const GTKEventController = require('./event-controller')

// Focus arrives through a controller added to a widget, as input does. A
// widget property says whether it has focus; only this says when it changed.
module.exports = exports = class GTKEventControllerFocus extends GTKEventController {
  static _events = {
    enter: binding.EVENT_CONTROLLER_FOCUS_EVENT_ENTER,
    leave: binding.EVENT_CONTROLLER_FOCUS_EVENT_LEAVE
  }

  _init() {
    return binding.eventControllerFocusInit()
  }
}
