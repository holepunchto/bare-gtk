const binding = require('../binding')
const GObject = require('./object')

module.exports = exports = class GDKFrameClock extends GObject {
  static _events = {
    afterPaint: binding.FRAME_CLOCK_EVENT_AFTER_PAINT,
    update: binding.FRAME_CLOCK_EVENT_UPDATE
  }

  _eventMask(mask) {
    binding.frameClockEventMask(this._tag, mask)
  }

  get frameCounter() {
    return binding.frameClockFrameCounter(this._tag)
  }

  get frameTime() {
    return binding.frameClockFrameTime(this._tag)
  }

  beginUpdating() {
    binding.frameClockBeginUpdating(this._tag)
    return this
  }

  endUpdating() {
    binding.frameClockEndUpdating(this._tag)
    return this
  }
}
