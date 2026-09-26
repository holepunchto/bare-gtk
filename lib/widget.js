const binding = require('../binding')
const { expose } = require('./handle')
const GObject = require('./object')
const wrap = require('./wrap')
const { hold, drop } = require('./reachable')

module.exports = exports = class GTKWidget extends GObject {
  get visible() {
    return binding.widgetVisible(this._tag)
  }

  set visible(value) {
    binding.widgetVisible(this._tag, value)
  }

  get opacity() {
    return binding.widgetOpacity(this._tag)
  }

  set opacity(value) {
    binding.widgetOpacity(this._tag, value)
  }

  get overflow() {
    return binding.widgetOverflow(this._tag)
  }

  set overflow(value) {
    binding.widgetOverflow(this._tag, value)
  }

  get sizeRequest() {
    return binding.widgetSizeRequest(this._tag)
  }

  set sizeRequest([width, height]) {
    binding.widgetSizeRequest(this._tag, width, height)
  }

  get parent() {
    return wrap(GTKWidget, binding.widgetParent(this._tag))
  }

  get firstChild() {
    return wrap(GTKWidget, binding.widgetFirstChild(this._tag))
  }

  get lastChild() {
    return wrap(GTKWidget, binding.widgetLastChild(this._tag))
  }

  get nextSibling() {
    return wrap(GTKWidget, binding.widgetNextSibling(this._tag))
  }

  get prevSibling() {
    return wrap(GTKWidget, binding.widgetPrevSibling(this._tag))
  }

  get layoutManager() {
    return wrap(GObject, binding.widgetLayoutManager(this._tag))
  }

  set layoutManager(manager) {
    drop(this, this.layoutManager)

    binding.widgetLayoutManager(this._tag, manager === null ? null : manager._tag)

    hold(this, manager)
  }

  get width() {
    return binding.widgetWidth(this._tag)
  }

  get height() {
    return binding.widgetHeight(this._tag)
  }

  grabFocus() {
    return binding.widgetGrabFocus(this._tag)
  }

  setRootFocus(widget) {
    binding.widgetSetRootFocus(this._tag, widget === null ? null : widget._tag)

    return this
  }

  addCssClass(name) {
    binding.widgetAddCssClass(this._tag, name)

    return this
  }

  removeCssClass(name) {
    binding.widgetRemoveCssClass(this._tag, name)

    return this
  }

  hasCssClass(name) {
    return binding.widgetHasCssClass(this._tag, name)
  }

  get canTarget() {
    return binding.widgetCanTarget(this._tag)
  }

  set canTarget(value) {
    binding.widgetCanTarget(this._tag, value)
  }

  get sensitive() {
    return binding.widgetSensitive(this._tag)
  }

  set sensitive(value) {
    binding.widgetSensitive(this._tag, value)
  }

  measure(orientation, forSize = -1) {
    return binding.widgetMeasure(this._tag, orientation, forSize)
  }

  computeBounds(target) {
    return binding.widgetComputeBounds(this._tag, target._tag)
  }

  insertAfter(parent, previousSibling = null) {
    binding.widgetInsertAfter(
      this._tag,
      parent._tag,
      previousSibling === null ? null : previousSibling._tag
    )

    return hold(parent, this)
  }

  insertBefore(parent, nextSibling = null) {
    binding.widgetInsertBefore(
      this._tag,
      parent._tag,
      nextSibling === null ? null : nextSibling._tag
    )

    return hold(parent, this)
  }

  addController(controller) {
    binding.widgetAddController(this._tag, controller._tag)

    return hold(this, controller)
  }

  removeController(controller) {
    binding.widgetRemoveController(this._tag, controller._tag)

    return drop(this, controller)
  }

  unparent() {
    const parent = this.parent

    binding.widgetUnparent(this._tag)

    return parent === null ? this : drop(parent, this)
  }
}

expose(exports)
