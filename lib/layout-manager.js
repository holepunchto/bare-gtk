const binding = require('../binding')
const GObject = require('./object')
const GTKLayoutChild = require('./layout-child')
const wrap = require('./wrap')

module.exports = exports = class GTKLayoutManager extends GObject {
  static layoutChild = GTKLayoutChild

  getLayoutChild(widget) {
    return wrap(
      this.constructor.layoutChild,
      binding.layoutManagerLayoutChild(this._tag, widget._tag)
    )
  }
}
