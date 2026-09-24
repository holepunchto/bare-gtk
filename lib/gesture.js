const binding = require('../binding')
const GTKEventController = require('./event-controller')

module.exports = exports = class GTKGesture extends GTKEventController {
  setState(state) {
    return binding.gestureSetState(this._tag, state)
  }
}
