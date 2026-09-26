const binding = require('../binding')
const { expose } = require('./handle')
const GTKWidget = require('./widget')

module.exports = exports = class GTKSwitch extends GTKWidget {
  _init() {
    return binding.switchInit()
  }

  _eventMask(mask) {
    binding.switchEventMask(this._tag, mask)
  }

  get active() {
    return binding.switchActive(this._tag)
  }

  set active(value) {
    binding.switchActive(this._tag, value)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: GTKSwitch }
    }
  }

  _onactive() {
    this.emit('active')
  }
}

exports._events = {
  active: binding.SWITCH_EVENT_ACTIVE
}

expose(exports)
