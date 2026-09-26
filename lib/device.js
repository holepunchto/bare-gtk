const binding = require('../binding')
const { expose } = require('./handle')
const GObject = require('./object')

module.exports = exports = class GTKDevice extends GObject {
  get source() {
    return binding.deviceSource(this._tag)
  }
}

expose(exports)
