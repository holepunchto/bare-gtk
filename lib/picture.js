const binding = require('../binding')
const { adopt, expose } = require('./handle')
const wrap = require('./wrap')
const GDKPaintable = require('./paintable')
const GTKWidget = require('./widget')

module.exports = exports = class GTKPicture extends GTKWidget {
  _init(opts) {
    return binding.pictureInit()
  }

  get paintable() {
    return wrap(GDKPaintable, binding.picturePaintable(this._tag))
  }

  set paintable(value) {
    binding.picturePaintable(this._tag, value === null ? null : adopt(value))
  }

  get keepAspectRatio() {
    return binding.pictureKeepAspectRatio(this._tag)
  }

  set keepAspectRatio(value) {
    binding.pictureKeepAspectRatio(this._tag, value)
  }

  get canShrink() {
    return binding.pictureCanShrink(this._tag)
  }

  set canShrink(value) {
    binding.pictureCanShrink(this._tag, value)
  }
}

expose(exports)
