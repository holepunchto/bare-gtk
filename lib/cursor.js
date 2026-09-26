const binding = require('../binding')
const { expose } = require('./handle')
const wrap = require('./wrap')
const GObject = require('./object')
const GDKTexture = require('./texture')

module.exports = exports = class GDKCursor extends GObject {
  static newFromName(name, fallback = null) {
    return wrap(
      GDKCursor,
      binding.cursorNewFromName(name, fallback === null ? null : fallback._tag)
    )
  }

  static newFromTexture(texture, hotspotX, hotspotY, fallback = null) {
    return wrap(
      GDKCursor,
      binding.cursorNewFromTexture(
        texture._tag,
        hotspotX,
        hotspotY,
        fallback === null ? null : fallback._tag
      )
    )
  }

  get name() {
    return binding.cursorName(this._tag)
  }

  get fallback() {
    return wrap(GDKCursor, binding.cursorFallback(this._tag))
  }

  get texture() {
    return wrap(GDKTexture, binding.cursorTexture(this._tag))
  }

  get hotspotX() {
    return binding.cursorHotspotX(this._tag)
  }

  get hotspotY() {
    return binding.cursorHotspotY(this._tag)
  }
}

expose(exports)
