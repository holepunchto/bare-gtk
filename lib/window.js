const binding = require('../binding')
const GTKWidget = require('./widget')

module.exports = exports = class GTKWindow extends GTKWidget {
  constructor() {
    super(binding.windowInit())
  }
}
