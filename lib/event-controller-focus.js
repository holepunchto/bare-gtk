const binding = require('../binding')
const GTKEventController = require('./event-controller')

module.exports = exports = class GTKEventControllerFocus extends GTKEventController {
  static _events = {
    enter: binding.EVENT_CONTROLLER_FOCUS_EVENT_ENTER,
    leave: binding.EVENT_CONTROLLER_FOCUS_EVENT_LEAVE
  }

  _init() {
    return binding.eventControllerFocusInit()
  }
}
