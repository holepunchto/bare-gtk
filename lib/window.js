const binding = require('../binding')
const GTKWidget = require('./widget')
const wrap = require('./wrap')
const { retain, release } = require('./retain')

module.exports = exports = class GTKWindow extends GTKWidget {
  static events = {
    'close-request': binding.WINDOW_EVENT_CLOSE_REQUEST
  }

  _init(opts) {
    return binding.windowInit()
  }

  _eventMask(mask) {
    binding.windowEventMask(this._tag, mask)
  }

  get title() {
    return binding.windowTitle(this._tag)
  }

  set title(value) {
    binding.windowTitle(this._tag, value)
  }

  get defaultSize() {
    return binding.windowDefaultSize(this._tag)
  }

  set defaultSize([width, height]) {
    binding.windowDefaultSize(this._tag, width, height)
  }

  get child() {
    return retain(this, wrap(GTKWidget, binding.windowChild(this._tag)))
  }

  set child(widget) {
    release(this, wrap(GTKWidget, binding.windowChild(this._tag)))

    binding.windowChild(this._tag, widget === null ? null : widget._tag)

    retain(this, widget)
  }
}
