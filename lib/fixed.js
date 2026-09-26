const binding = require('../binding')
const { expose } = require('./handle')
const GTKWidget = require('./widget')

module.exports = exports = class GTKFixed extends GTKWidget {
  _init(opts) {
    return binding.fixedInit()
  }
}

expose(exports)
