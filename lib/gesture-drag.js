const binding = require('../binding')
const { expose } = require('./handle')
const GTKGestureSingle = require('./gesture-single')

module.exports = exports = class GTKGestureDrag extends GTKGestureSingle {
  static _events = {
    'drag-begin': binding.GESTURE_DRAG_EVENT_DRAG_BEGIN,
    'drag-update': binding.GESTURE_DRAG_EVENT_DRAG_UPDATE,
    'drag-end': binding.GESTURE_DRAG_EVENT_DRAG_END,
    cancel: binding.GESTURE_DRAG_EVENT_CANCEL
  }

  _init() {
    return binding.gestureDragInit()
  }

  get startPoint() {
    return binding.gestureDragStartPoint(this._tag)
  }

  get offset() {
    return binding.gestureDragOffset(this._tag)
  }
}

expose(exports)
