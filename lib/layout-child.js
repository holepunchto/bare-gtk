const binding = require('../binding')
const { expose } = require('./handle')
const GObject = require('./object')
const GTKWidget = require('./widget')
const wrap = require('./wrap')

module.exports = exports = class GTKLayoutChild extends GObject {
  get childWidget() {
    return wrap(GTKWidget, binding.layoutChildChildWidget(this._tag))
  }
}

expose(exports)
