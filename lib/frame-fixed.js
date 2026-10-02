const BareFrameLayout = require('./frame-layout')
const GTKFixed = require('./fixed')

module.exports = exports = class BareFrameFixed extends GTKFixed {
  constructor(opts = {}) {
    super(opts)

    const { tag = null } = opts

    if (tag === null) this.layoutManager = new BareFrameLayout()
  }
}
