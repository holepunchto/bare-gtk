const binding = require('../binding')
const wrap = require('./wrap')
const GObject = require('./object')

module.exports = exports = class GTKApplication extends GObject {
  static getDefault() {
    return wrap(GTKApplication, binding.applicationGetDefault())
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
