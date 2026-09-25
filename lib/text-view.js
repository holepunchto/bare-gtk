const binding = require('../binding')
const wrap = require('./wrap')
const GTKTextBuffer = require('./text-buffer')
const GTKWidget = require('./widget')

module.exports = exports = class GTKTextView extends GTKWidget {
  static _events = {
    'has-focus': binding.TEXT_VIEW_EVENT_HAS_FOCUS,
    'is-focus': binding.TEXT_VIEW_EVENT_IS_FOCUS
  }

  _init(opts) {
    return binding.textViewInit()
  }

  _eventMask(mask) {
    binding.textViewEventMask(this._tag, mask)
  }

  get buffer() {
    return wrap(GTKTextBuffer, binding.textViewBuffer(this._tag))
  }

  get editable() {
    return binding.textViewEditable(this._tag)
  }

  set editable(value) {
    binding.textViewEditable(this._tag, value)
  }

  get wrapMode() {
    return binding.textViewWrapMode(this._tag)
  }

  set wrapMode(value) {
    binding.textViewWrapMode(this._tag, value)
  }

  get inputPurpose() {
    return binding.textViewInputPurpose(this._tag)
  }

  set inputPurpose(value) {
    binding.textViewInputPurpose(this._tag, value)
  }

  get inputHints() {
    return binding.textViewInputHints(this._tag)
  }

  set inputHints(value) {
    binding.textViewInputHints(this._tag, value)
  }
}
