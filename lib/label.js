const binding = require('../binding')
const GTKWidget = require('./widget')

module.exports = exports = class GTKLabel extends GTKWidget {
  _init(opts) {
    return binding.labelInit()
  }

  get text() {
    return binding.labelText(this._tag)
  }

  set text(value) {
    binding.labelText(this._tag, value)
  }

  get wrap() {
    return binding.labelWrap(this._tag)
  }

  set wrap(value) {
    binding.labelWrap(this._tag, value)
  }

  get wrapMode() {
    return binding.labelWrapMode(this._tag)
  }

  set wrapMode(value) {
    binding.labelWrapMode(this._tag, value)
  }

  get xalign() {
    return binding.labelXalign(this._tag)
  }

  set xalign(value) {
    binding.labelXalign(this._tag, value)
  }

  get yalign() {
    return binding.labelYalign(this._tag)
  }

  set yalign(value) {
    binding.labelYalign(this._tag, value)
  }

  get justify() {
    return binding.labelJustify(this._tag)
  }

  set justify(value) {
    binding.labelJustify(this._tag, value)
  }
}
