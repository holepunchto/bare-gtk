const binding = require('../binding')
const GTKWidget = require('./widget')

module.exports = exports = class GTKFixed extends GTKWidget {
  _init(opts) {
    return binding.fixedInit()
  }
}
