const binding = require('../binding')

const handle = (exports.handle = Symbol.for('bare.gobject.handle'))

exports.expose = function expose(classes) {
  for (const constructor of classes) {
    constructor.prototype[handle] = function () {
      return binding.handle(this._tag)
    }
  }
}

exports.adopt = function adopt(object) {
  if (object === null || typeof object[handle] !== 'function') {
    throw new TypeError('Object does not implement the GObject handle protocol')
  }

  return binding.adopt(object[handle]())
}
