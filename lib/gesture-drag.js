const binding = require('../binding')
const GTKGestureSingle = require('./gesture-single')

module.exports = exports = class GTKGestureDrag extends GTKGestureSingle {
  static _events = {
    dragBegin: binding.GESTURE_DRAG_EVENT_DRAG_BEGIN,
    dragUpdate: binding.GESTURE_DRAG_EVENT_DRAG_UPDATE,
    dragEnd: binding.GESTURE_DRAG_EVENT_DRAG_END,
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
