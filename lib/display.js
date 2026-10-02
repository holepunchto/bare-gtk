const binding = require('../binding')
const GObject = require('./object')
const GDKMonitor = require('./monitor')
const GDKClipboard = require('./clipboard')
const wrap = require('./wrap')

module.exports = exports = class GDKDisplay extends GObject {
  static getDefault() {
    return wrap(GDKDisplay, binding.displayGetDefault())
  }

  getMonitorAtSurface(surface) {
    return wrap(GDKMonitor, binding.displayGetMonitorAtSurface(this._tag, surface._tag))
  }

  getClipboard() {
    return wrap(GDKClipboard, binding.displayGetClipboard(this._tag))
  }

  getPrimaryClipboard() {
    return wrap(GDKClipboard, binding.displayGetPrimaryClipboard(this._tag))
  }
}
