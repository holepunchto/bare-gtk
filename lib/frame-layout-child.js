const binding = require('../binding')
const GTKLayoutChild = require('./layout-child')

module.exports = exports = class BareFrameLayoutChild extends GTKLayoutChild {
  get frame() {
    return binding.frameLayoutChildFrame(this._tag)
  }

  set frame([x, y, width, height]) {
    binding.frameLayoutChildFrame(this._tag, x, y, width, height)
  }
}
