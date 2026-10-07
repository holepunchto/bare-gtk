const { test } = require('bare-tap')
const { afterAnimationFrame } = require('bare-animation-frame')
const { FrameFixed, FrameLayout, Label } = require('..')
const { mount } = require('./helpers')

test('keeps the layout manager wrapper', (t) => {
  const fixed = new FrameFixed()
  const manager = fixed.layoutManager

  t.ok(manager instanceof FrameLayout, 'a frame layout')
  t.ok(fixed.layoutManager === manager, 'same wrapper')
})

test('places a child at its frame', async (t) => {
  const fixed = new FrameFixed()
  const child = new Label()

  child.insertBefore(fixed)

  const layout = fixed.layoutManager.getLayoutChild(child)

  t.ok(layout.childWidget === child, 'the layout child knows its widget')
  t.ok(fixed.layoutManager.getLayoutChild(child) === layout, 'same wrapper')

  layout.frame = [10, 20, 30, 40]

  t.deepStrictEqual(layout.frame, [10, 20, 30, 40], 'frame')

  await mount(t, fixed)

  t.deepStrictEqual(child.computeBounds(fixed), [10, 20, 30, 40], 'placed')
})

test('emits the size it is laid out at', async (t) => {
  const fixed = new FrameFixed()

  const sizes = []

  fixed.layoutManager.on('resize', (width, height) => sizes.push([width, height]))

  await mount(t, fixed)

  t.ok(sizes.length > 0, 'laid out')
  t.deepStrictEqual(sizes[sizes.length - 1], [fixed.width, fixed.height], 'at its size')
})

test('applies frames set while laying out', async (t) => {
  const fixed = new FrameFixed()
  const child = new Label()

  child.insertBefore(fixed)

  const layout = fixed.layoutManager.getLayoutChild(child)

  fixed.layoutManager.on('resize', (width, height) => {
    layout.frame = [0, 0, width / 2, height / 2]
  })

  await mount(t, fixed)

  t.equal(child.width, fixed.width / 2, 'half the width')
  t.equal(child.height, fixed.height / 2, 'half the height')
})

test('moves a child with a transform', async (t) => {
  const fixed = new FrameFixed()
  const child = new Label()

  child.insertBefore(fixed)

  const layout = fixed.layoutManager.getLayoutChild(child)

  layout.frame = [10, 10, 20, 20]

  await mount(t, fixed)

  // prettier-ignore
  layout.transform = [
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    5, 15, 0, 1
  ]

  await afterAnimationFrame()

  t.deepStrictEqual(child.computeBounds(fixed), [15, 25, 20, 20])
})
