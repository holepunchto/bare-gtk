const { afterAnimationFrame } = require('bare-animation-frame')
const { Window } = require('..')

// A window cannot be closed, so the tests share one and swap its child.
let window = null

exports.mount = async function mount(t, child) {
  if (window === null) {
    window = new Window()
    window.defaultSize = [400, 300]
    window.visible = true
  }

  window.child = child

  t.teardown(() => {
    window.child = null
  })

  // GTK only lays out and draws new content on its next frame.
  await afterAnimationFrame()

  return window
}
