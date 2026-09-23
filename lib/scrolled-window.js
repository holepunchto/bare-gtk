const binding = require('../binding')
const GTKWidget = require('./widget')
const GTKAdjustment = require('./adjustment')
const wrap = require('./wrap')
const { hold, drop } = require('./reachable')

module.exports = exports = class GTKScrolledWindow extends GTKWidget {
  _init() {
    return binding.scrolledWindowInit()
  }

  get child() {
    return wrap(GTKWidget, binding.scrolledWindowChild(this._tag))
  }

  set child(child) {
    drop(this, this.child)

    binding.scrolledWindowChild(this._tag, child === null ? null : child._tag)

    hold(this, child)
  }

  get hadjustment() {
    return wrap(GTKAdjustment, binding.scrolledWindowAdjustment(this._tag, true))
  }

  get vadjustment() {
    return wrap(GTKAdjustment, binding.scrolledWindowAdjustment(this._tag, false))
  }

  setPolicy(horizontal, vertical) {
    binding.scrolledWindowPolicy(this._tag, horizontal, vertical)

    return this
  }
}
