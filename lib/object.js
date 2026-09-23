const EventEmitter = require('bare-events')
const binding = require('../binding')
const observe = require('./events')

module.exports = exports = class GObject extends EventEmitter {
  constructor(opts = {}) {
    super()

    const { tag = null } = opts

    this._retained = new Set()

    const created = tag === null ? this._init(opts) : tag

    this._tag = typeof created === 'number' ? created : binding.adopt(created)

    this._token = binding.claim(this._tag, this)

    observe(this)
  }

  _init(opts) {
    throw new TypeError(`${this.constructor.name} cannot be constructed directly`)
  }

  _eventMask(mask) {}
}
