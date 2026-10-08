const { test } = require('bare-tap')
const { Fixed, Label, constants } = require('..')
const { mount } = require('./helpers')

const { ORIENTATION_HORIZONTAL, ORIENTATION_VERTICAL } = constants

test('builds a hierarchy', (t) => {
  const parent = new Fixed()
  const a = new Label()
  const b = new Label()
  const c = new Label()

  t.equal(a.parent, null, 'no parent')
  t.equal(parent.firstChild, null, 'no children')

  a.insertBefore(parent)
  c.insertBefore(parent)
  b.insertAfter(parent, a)

  t.ok(a.parent === parent, 'same parent wrapper')
  t.ok(parent.firstChild === a, 'first child')
  t.ok(parent.lastChild === c, 'last child')
  t.ok(a.nextSibling === b, 'inserted after')
  t.ok(c.prevSibling === b, 'and before')

  b.unparent()

  t.equal(b.parent, null, 'removed')
  t.ok(a.nextSibling === c, 'siblings closed up')
})

test('inserts first and last', (t) => {
  const parent = new Fixed()
  const a = new Label()
  const b = new Label()
  const c = new Label()

  a.insertBefore(parent)
  b.insertAfter(parent)
  c.insertBefore(parent, null)

  t.ok(parent.firstChild === b, 'after nothing is first')
  t.ok(parent.lastChild === c, 'before nothing is last')
})

test('sets the common properties', (t) => {
  const widget = new Label()

  t.equal(widget.visible, true, 'visible')
  t.equal(widget.sensitive, true, 'sensitive')
  t.equal(widget.canTarget, true, 'targetable')
  t.equal(widget.opacity, 1, 'opaque')

  widget.visible = false
  widget.sensitive = false
  widget.canTarget = false

  // GTK keeps opacity in eight bits, so this is a value it can hold exactly.
  widget.opacity = 0.2

  t.equal(widget.visible, false, 'hidden')
  t.equal(widget.sensitive, false, 'insensitive')
  t.equal(widget.canTarget, false, 'untargetable')
  t.equal(widget.opacity, 0.2, 'translucent')
})

test('sets the overflow', (t) => {
  const widget = new Fixed()

  widget.overflow = constants.OVERFLOW_HIDDEN

  t.equal(widget.overflow, constants.OVERFLOW_HIDDEN)

  widget.overflow = constants.OVERFLOW_VISIBLE

  t.equal(widget.overflow, constants.OVERFLOW_VISIBLE)
})

test('sets the size request', (t) => {
  const widget = new Fixed()

  t.deepStrictEqual(widget.sizeRequest, [-1, -1], 'none')

  widget.sizeRequest = [30, 40]

  t.deepStrictEqual(widget.sizeRequest, [30, 40], 'requested')

  const [minimum] = widget.measure(ORIENTATION_HORIZONTAL)

  t.equal(minimum, 30, 'measured')
})

test('adds and removes CSS classes', (t) => {
  const widget = new Label()

  t.equal(widget.hasCssClass('title'), false, 'not before')

  widget.addCssClass('title')

  t.equal(widget.hasCssClass('title'), true, 'added')

  widget.removeCssClass('title')

  t.equal(widget.hasCssClass('title'), false, 'removed')
})

test('measures for a size along the other axis', (t) => {
  const label = new Label()

  label.text = 'A string long enough to wrap onto more than one line when it is narrow'
  label.wrap = true

  const [, wide] = label.measure(ORIENTATION_VERTICAL, 1000)
  const [, narrow] = label.measure(ORIENTATION_VERTICAL, 100)

  t.ok(narrow > wide, 'taller when narrower')
})

test('is laid out in a window', async (t) => {
  const label = new Label()

  label.text = 'Hello'

  const window = await mount(t, label)

  t.ok(label.native === window, 'the window is its native')
  t.ok(label.width > 0 && label.height > 0, 'has a size')
  t.ok(label.scaleFactor >= 1, 'a scale factor')
  t.ok(label.frameClock !== null, 'a frame clock')

  const bounds = label.computeBounds(window)

  t.equal(bounds.length, 4, 'bounds in the window')
  t.equal(bounds[2], label.width, 'the width it was given')
  t.equal(label.computeBounds(new Label()), null, 'no bounds against an unrelated widget')
})

test('has no frame clock before it is realized', (t) => {
  t.equal(new Label().frameClock, null)
})

test('sets the cursor', (t) => {
  const widget = new Label()

  t.equal(widget.cursor, null, 'none')

  widget.setCursorFromName('pointer')

  t.equal(widget.cursor.name, 'pointer', 'by name')

  widget.cursor = null

  t.equal(widget.cursor, null, 'cleared')
})
