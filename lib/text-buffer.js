const binding = require('../binding')
const GObject = require('./object')

module.exports = exports = class GTKTextBuffer extends GObject {
  static _events = {
    changed: binding.TEXT_BUFFER_EVENT_CHANGED,
    cursorPosition: binding.TEXT_BUFFER_EVENT_CURSOR_POSITION,
    insertText: binding.TEXT_BUFFER_EVENT_INSERT_TEXT,
    deleteRange: binding.TEXT_BUFFER_EVENT_DELETE_RANGE
  }

  _eventMask(mask) {
    binding.textBufferEventMask(this._tag, mask)
  }

  get text() {
    return binding.textBufferText(this._tag)
  }

  set text(value) {
    binding.textBufferText(this._tag, value)
  }

  get selectionBounds() {
    return binding.textBufferSelectionBounds(this._tag)
  }

  selectRange(start, end) {
    binding.textBufferSelectRange(this._tag, start, end)

    return this
  }
}
