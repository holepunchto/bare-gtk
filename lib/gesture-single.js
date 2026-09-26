const binding = require('../binding')
const { expose } = require('./handle')
const GTKGesture = require('./gesture')

module.exports = exports = class GTKGestureSingle extends GTKGesture {
  get button() {
    return binding.gestureSingleButton(this._tag)
  }

  set button(button) {
    binding.gestureSingleButton(this._tag, button)
  }
}

expose(exports)
