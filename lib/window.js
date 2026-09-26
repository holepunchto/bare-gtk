const binding = require('../binding')
const { expose } = require('./handle')
const GTKWidget = require('./widget')
const wrap = require('./wrap')
const { hold, drop } = require('./reachable')

module.exports = exports = class GTKWindow extends GTKWidget {
  static _events = {
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
    return hold(this, wrap(GTKWidget, binding.windowChild(this._tag)))
  }

  set child(widget) {
    drop(this, wrap(GTKWidget, binding.windowChild(this._tag)))

    binding.windowChild(this._tag, widget === null ? null : widget._tag)

    hold(this, widget)
  }
}

expose(exports)
