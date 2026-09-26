const binding = require('../binding')
const { expose } = require('./handle')
const GTKWidget = require('./widget')

module.exports = exports = class GTKSpinner extends GTKWidget {
  _init() {
    return binding.spinnerInit()
  }

  get spinning() {
    return binding.spinnerSpinning(this._tag)
  }

  set spinning(value) {
    binding.spinnerSpinning(this._tag, value)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: GTKSpinner }
    }
  }
}

expose(exports)
