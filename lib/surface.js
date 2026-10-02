const binding = require('../binding')
const wrap = require('./wrap')
const GObject = require('./object')
const GDKDisplay = require('./display')

module.exports = exports = class GDKSurface extends GObject {
  get width() {
    return binding.surfaceWidth(this._tag)
  }

  get height() {
    return binding.surfaceHeight(this._tag)
  }

  get scaleFactor() {
    return binding.surfaceScaleFactor(this._tag)
  }

  get display() {
    return wrap(GDKDisplay, binding.surfaceDisplay(this._tag))
  }
}
