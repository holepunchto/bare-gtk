const binding = require('../binding')
const wrap = require('./wrap')
const GObject = require('./object')
const GTKWindow = require('./window')

module.exports = exports = class GTKApplication extends GObject {
  static getDefault() {
    return wrap(GTKApplication, binding.applicationGetDefault())
  }

  get activeWindow() {
    return wrap(GTKWindow, binding.applicationActiveWindow(this._tag))
  }

  hold() {
    binding.applicationHold(this._tag)
    return this
  }

  release() {
    binding.applicationRelease(this._tag)
    return this
  }
}
