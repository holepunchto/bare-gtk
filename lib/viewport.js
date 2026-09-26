const binding = require('../binding')
const { expose } = require('./handle')
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

  get hscrollPolicy() {
    return binding.viewportHscrollPolicy(this._tag)
  }

  set hscrollPolicy(policy) {
    binding.viewportHscrollPolicy(this._tag, policy)
  }

  get vscrollPolicy() {
    return binding.viewportVscrollPolicy(this._tag)
  }

  set vscrollPolicy(policy) {
    binding.viewportVscrollPolicy(this._tag, policy)
  }
}

expose(exports)
