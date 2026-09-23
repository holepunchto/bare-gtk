const binding = require('../binding')
const GObject = require('./object')

module.exports = exports = class GDKMonitor extends GObject {
  get geometry() {
    return binding.monitorGeometry(this._tag)
  }

  get scaleFactor() {
    return binding.monitorScaleFactor(this._tag)
  }

  get widthMm() {
    return binding.monitorWidthMm(this._tag)
  }

  get heightMm() {
    return binding.monitorHeightMm(this._tag)
  }

  get refreshRate() {
    return binding.monitorRefreshRate(this._tag)
  }

  get connector() {
    return binding.monitorConnector(this._tag)
  }

  get manufacturer() {
    return binding.monitorManufacturer(this._tag)
  }

  get model() {
    return binding.monitorModel(this._tag)
  }

  get valid() {
    return binding.monitorValid(this._tag)
  }
}
