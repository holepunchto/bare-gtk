const binding = require('../binding')
const { expose } = require('./handle')
const GObject = require('./object')

module.exports = exports = class GTKCssProvider extends GObject {
  _init(opts) {
    return binding.cssProviderInit()
  }

  loadFromData(data) {
    binding.cssProviderLoadFromData(this._tag, data)

    return this
  }
}

expose(exports)
