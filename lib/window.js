const binding = require('../binding')
const GTKWidget = require('./widget')

module.exports = exports = class GTKWindow extends GTKWidget {
  constructor() {
    super()

    this._handle = binding.windowInit(this)
  }

  get child() {
    return new GTKWidget(binding.windowChild(this._handle))
  }

  set child(widget) {
    binding.windowChild(this._handle, widget._handle)
  }
}
