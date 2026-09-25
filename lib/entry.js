const binding = require('../binding')
const GTKWidget = require('./widget')

// A `GtkEntry` is the single line half of a text input; what it holds and what
// can be done to it is `GtkEditable`, which it implements.
module.exports = exports = class GTKEntry extends GTKWidget {
  static _events = {
    changed: binding.ENTRY_EVENT_CHANGED,
    activate: binding.ENTRY_EVENT_ACTIVATE,
    'cursor-position': binding.ENTRY_EVENT_CURSOR_POSITION,
    'insert-text': binding.ENTRY_EVENT_INSERT_TEXT,
    'delete-text': binding.ENTRY_EVENT_DELETE_TEXT,
    'has-focus': binding.ENTRY_EVENT_HAS_FOCUS,
    'is-focus': binding.ENTRY_EVENT_IS_FOCUS
  }

  _init(opts) {
    return binding.entryInit()
  }

  _eventMask(mask) {
    binding.entryEventMask(this._tag, mask)
  }

  get text() {
    return binding.editableText(this._tag)
  }

  set text(value) {
    binding.editableText(this._tag, value)
  }

  get editable() {
    return binding.editableEditable(this._tag)
  }

  set editable(value) {
    binding.editableEditable(this._tag, value)
  }

  get selectionBounds() {
    return binding.editableSelectionBounds(this._tag)
  }

  selectRegion(start, end) {
    binding.editableSelectRegion(this._tag, start, end)

    return this
  }

  grabFocusWithoutSelecting() {
    return binding.entryGrabFocusWithoutSelecting(this._tag)
  }

  get placeholderText() {
    return binding.entryPlaceholderText(this._tag)
  }

  set placeholderText(value) {
    binding.entryPlaceholderText(this._tag, value)
  }

  get visibility() {
    return binding.entryVisibility(this._tag)
  }

  set visibility(value) {
    binding.entryVisibility(this._tag, value)
  }

  get inputPurpose() {
    return binding.entryInputPurpose(this._tag)
  }

  set inputPurpose(value) {
    binding.entryInputPurpose(this._tag, value)
  }

  get inputHints() {
    return binding.entryInputHints(this._tag)
  }

  set inputHints(value) {
    binding.entryInputHints(this._tag, value)
  }
}

exports.INPUT_PURPOSE = {
  FREE_FORM: 0,
  ALPHA: 1,
  DIGITS: 2,
  NUMBER: 3,
  PHONE: 4,
  URL: 5,
  EMAIL: 6,
  NAME: 7,
  PASSWORD: 8,
  PIN: 9,
  TERMINAL: 10
}

exports.INPUT_HINTS = {
  NONE: 0,
  SPELLCHECK: 1 << 0,
  NO_SPELLCHECK: 1 << 1,
  WORD_COMPLETION: 1 << 2,
  LOWERCASE: 1 << 3,
  UPPERCASE_CHARS: 1 << 4,
  UPPERCASE_WORDS: 1 << 5,
  UPPERCASE_SENTENCES: 1 << 6,
  INHIBIT_OSK: 1 << 7
}
