const binding = require('../binding')
const GTKWidget = require('./widget')

module.exports = exports = class GTKSwitch extends GTKWidget {
  static _events = {
    active: binding.SWITCH_EVENT_ACTIVE
  }

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
}
