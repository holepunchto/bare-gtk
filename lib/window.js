const binding = require('../binding')
const GTKWidget = require('./widget')

module.exports = exports = class GTKWindow extends GTKWidget {
  constructor() {
    super()

    this._handle = binding.windowInit(this)
  }

  get defaultSize() {
    return binding.windowDefaultSize(this._handle)
  }

  set defaultSize([width, height]) {
    binding.windowDefaultSize(this._handle, width, height)
  }

  get child() {
    const handle = binding.windowChild(this._handle)

    if (handle === null) return null

    return new GTKWidget(handle)
  }

  set child(widget) {
    binding.windowChild(this._handle, widget._handle)
  }
}
