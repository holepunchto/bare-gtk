const binding = require('../binding')
const { expose } = require('./handle')
const BareFrameLayoutChild = require('./frame-layout-child')
const GTKLayoutManager = require('./layout-manager')

module.exports = exports = class BareFrameLayout extends GTKLayoutManager {
  static layoutChild = BareFrameLayoutChild

  static _events = {
    resize: binding.FRAME_LAYOUT_EVENT_RESIZE
  }

  _init(opts) {
    return binding.frameLayoutInit()
  }

  _eventMask(mask) {
    binding.frameLayoutEventMask(this._tag, mask)
  }
}

expose(exports)
