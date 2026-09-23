const binding = require('../binding')
const GObject = require('./object')

module.exports = exports = class GDKClipboard extends GObject {
  readTextAsync(callback) {
    binding.clipboardReadTextAsync(this._tag, callback)
  }

  setText(text) {
    binding.clipboardSetText(this._tag, text)

    return this
  }

  get formats() {
    return binding.clipboardFormats(this._tag)
  }
}
