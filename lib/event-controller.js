const binding = require('../binding')
const { expose } = require('./handle')
const GObject = require('./object')
const wrap = require('./wrap')
const GTKDevice = require('./device')

module.exports = exports = class GTKEventController extends GObject {
  _eventMask(mask) {
    binding.eventControllerEventMask(this._tag, mask)
  }

  get propagationPhase() {
    return binding.eventControllerPropagationPhase(this._tag)
  }

  set propagationPhase(phase) {
    binding.eventControllerPropagationPhase(this._tag, phase)
  }

  get currentEventDevice() {
    return wrap(GTKDevice, binding.eventControllerCurrentEventDevice(this._tag))
  }
}

expose(exports)
