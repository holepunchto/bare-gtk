const binding = require('../binding')
const { expose } = require('./handle')
const GObject = require('./object')
const wrap = require('./wrap')

module.exports = exports = class GDKDisplay extends GObject {
  static getDefault() {
    return wrap(GDKDisplay, binding.displayGetDefault())
  }
}

expose(exports)
