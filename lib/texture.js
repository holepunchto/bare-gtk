const binding = require('../binding')
const { expose } = require('./handle')
const wrap = require('./wrap')
const GDKPaintable = require('./paintable')

module.exports = exports = class GDKTexture extends GDKPaintable {
  static newFromFilename(path) {
    return wrap(GDKTexture, binding.textureNewFromFilename(path))
  }

  get width() {
    return binding.textureWidth(this._tag)
  }

  get height() {
    return binding.textureHeight(this._tag)
  }
}

expose(exports)
