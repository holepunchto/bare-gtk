const binding = require('../binding')
const { expose } = require('./handle')
const GTKEventController = require('./event-controller')

module.exports = exports = class GTKEventControllerMotion extends GTKEventController {
  static _events = {
    motion: binding.EVENT_CONTROLLER_MOTION_EVENT_MOTION,
    enter: binding.EVENT_CONTROLLER_MOTION_EVENT_ENTER,
    leave: binding.EVENT_CONTROLLER_MOTION_EVENT_LEAVE
  }

  _init() {
    return binding.eventControllerMotionInit()
  }
}

expose(exports)
