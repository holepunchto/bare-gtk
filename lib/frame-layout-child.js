const binding = require('../binding')
const { expose } = require('./handle')
const GTKLayoutChild = require('./layout-child')

module.exports = exports = class BareFrameLayoutChild extends GTKLayoutChild {
  get frame() {
    return binding.frameLayoutChildFrame(this._tag)
  }

  set frame([x, y, width, height]) {
    binding.frameLayoutChildFrame(this._tag, x, y, width, height)
  }

  set transform(value) {
    binding.frameLayoutChildTransform(this._tag, value)
  }
}

expose(exports)
