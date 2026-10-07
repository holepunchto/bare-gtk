const { test } = require('bare-tap')
const { Label, Window } = require('..')
const Application = require('../lib/application')
const Display = require('../lib/display')
const { mount } = require('./helpers')

test('sets the title and default size', (t) => {
  const window = new Window()

  t.equal(window.visible, false, 'hidden')
  t.equal(window.title, null, 'no title')
  t.equal(window.surface, null, 'no surface before it is shown')

  window.title = 'Hello'
  window.defaultSize = [300, 200]

  t.equal(window.title, 'Hello', 'title')
  t.deepStrictEqual(window.defaultSize, [300, 200], 'default size')
})

test('keeps the child wrapper', (t) => {
  const window = new Window()
  const label = new Label()

  t.equal(window.child, null, 'no child')

  window.child = label

  t.ok(window.child === label, 'same wrapper')
  t.ok(label.parent === window, 'the child knows its window')

  window.child = null

  t.equal(label.parent, null, 'removed')
})

test('draws on a surface once shown', async (t) => {
  const window = await mount(t, new Label())

  const surface = window.surface

  t.ok(surface !== null, 'a surface')
  t.ok(window.surface === surface, 'same wrapper')
  t.equal(surface.width, window.width, 'as wide as the window')
  t.equal(surface.height, window.height, 'as tall as the window')
  t.ok(surface.scaleFactor >= 1, 'a scale factor')
  t.ok(surface.display === Display.getDefault(), 'on the default display')
})

test('is known to the application', async (t) => {
  const window = await mount(t, new Label())

  const application = Application.getDefault()

  t.ok(application !== null, 'started by the runtime')
  t.ok(Application.getDefault() === application, 'same wrapper')
  t.ok(application.activeWindow === window, 'the active window')
})

test('is on a monitor', async (t) => {
  const window = await mount(t, new Label())

  const monitor = Display.getDefault().getMonitorAtSurface(window.surface)

  t.ok(monitor !== null, 'a monitor')
  t.equal(monitor.valid, true, 'connected')
  t.ok(monitor.geometry.width > 0 && monitor.geometry.height > 0, 'has a size')
  t.ok(monitor.scaleFactor >= 1, 'a scale factor')
})
