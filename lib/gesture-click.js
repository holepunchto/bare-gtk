const binding = require('../binding')
const { expose } = require('./handle')
const GTKGestureSingle = require('./gesture-single')

module.exports = exports = class GTKGestureClick extends GTKGestureSingle {
  static _events = {
    pressed: binding.GESTURE_CLICK_EVENT_PRESSED,
    released: binding.GESTURE_CLICK_EVENT_RELEASED,
    cancel: binding.GESTURE_CLICK_EVENT_CANCEL
  }

  _init() {
    return binding.gestureClickInit()
  }
}

expose(exports)
