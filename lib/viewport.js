const binding = require('../binding')
const GTKWidget = require('./widget')
const wrap = require('./wrap')
const { hold, drop } = require('./reachable')

module.exports = exports = class GTKViewport extends GTKWidget {
  _init() {
    return binding.viewportInit()
  }

  get child() {
    return wrap(GTKWidget, binding.viewportChild(this._tag))
  }

  set child(child) {
    drop(this, this.child)

    binding.viewportChild(this._tag, child === null ? null : child._tag)

    hold(this, child)
  }

  setScrollPolicy(horizontal, vertical) {
    binding.viewportScrollPolicy(this._tag, horizontal, vertical)

    return this
  }
}
