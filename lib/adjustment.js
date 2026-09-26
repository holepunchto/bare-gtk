const binding = require('../binding')
const { expose } = require('./handle')
const GObject = require('./object')

module.exports = exports = class GTKAdjustment extends GObject {
  static _events = {
    'value-changed': binding.ADJUSTMENT_EVENT_VALUE_CHANGED
  }

  _eventMask(mask) {
    binding.adjustmentEventMask(this._tag, mask)
  }

  get value() {
    return binding.adjustmentValue(this._tag)
  }

  set value(value) {
    binding.adjustmentValue(this._tag, value)
  }

  get upper() {
    return binding.adjustmentUpper(this._tag)
  }
}

expose(exports)
