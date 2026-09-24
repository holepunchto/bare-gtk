const binding = require('../binding')
const GObject = require('./object')

module.exports = exports = class GTKDevice extends GObject {
  get source() {
    return binding.deviceSource(this._tag)
  }
}
