const { test } = require('bare-tap')
const { Fixed, ScrolledWindow, Viewport, constants } = require('..')
const { mount } = require('./helpers')

function scrolling() {
  const scroller = new ScrolledWindow()
  const viewport = new Viewport()
  const content = new Fixed()

  content.sizeRequest = [1000, 2000]

  viewport.child = content
  scroller.child = viewport

  return { scroller, viewport, content }
}

test('holds its child', (t) => {
  const { scroller, viewport, content } = scrolling()

  t.ok(scroller.child === viewport, 'the viewport')
  t.ok(viewport.child === content, 'the content')

  viewport.child = null

  t.equal(viewport.child, null, 'removed')
})

test('keeps the adjustment wrappers', (t) => {
  const { scroller } = scrolling()

  t.ok(scroller.hadjustment === scroller.hadjustment, 'horizontal')
  t.ok(scroller.vadjustment === scroller.vadjustment, 'vertical')
  t.ok(scroller.hadjustment !== scroller.vadjustment, 'one for each axis')
})

test('scrolls by moving an adjustment', async (t) => {
  const { scroller } = scrolling()

  await mount(t, scroller)

  const adjustment = scroller.vadjustment

  t.equal(adjustment.upper, 2000, 'the height of the content')

  const values = []

  adjustment.on('value-changed', (value) => values.push(value))
  adjustment.value = 100

  t.equal(adjustment.value, 100, 'scrolled')
  t.deepStrictEqual(values, [100], 'emitted')

  adjustment.value = 1e6

  t.equal(adjustment.value, 2000 - scroller.height, 'clamped to the end')
})

test('sets the policies', (t) => {
  const { scroller, viewport } = scrolling()

  scroller.setPolicy(constants.POLICY_EXTERNAL, constants.POLICY_AUTOMATIC)

  viewport.hscrollPolicy = constants.SCROLL_NATURAL
  viewport.vscrollPolicy = constants.SCROLL_MINIMUM

  t.equal(viewport.hscrollPolicy, constants.SCROLL_NATURAL, 'horizontal')
  t.equal(viewport.vscrollPolicy, constants.SCROLL_MINIMUM, 'vertical')
})
