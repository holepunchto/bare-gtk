const EventEmitter = require('bare-events')
const binding = require('../binding')

module.exports = class GTKWidget extends EventEmitter {
  constructor(handle = null) {
    super()

    this._handle = handle
  }

  get visible() {
    return binding.widgetVisible(this._handle)
  }

  set visible(value) {
    binding.widgetVisible(this._handle, value)
  }

  get sizeRequest() {
    return binding.widgetSizeRequest(this._handle)
  }

  set sizeRequest([width, height]) {
    binding.widgetSizeRequest(this._handle, width, height)
  }
}
