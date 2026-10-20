const AnimationFrameProvider = require('bare-animation-frame')
const { Window } = require('..')

// A window cannot be closed, so the tests share one and swap its child.
let window = null
let frames = null

function shared() {
  if (window === null) {
    window = new Window()
    window.defaultSize = [400, 300]
    window.visible = true

    // Without a window manager, the active window is whichever was made last,
    // and the tests make hidden ones that never draw.
    frames = new AnimationFrameProvider(window)
  }

  return window
}

exports.mount = async function mount(t, child) {
  const window = shared()

  window.child = child

  t.teardown(() => {
    window.child = null
  })

  // GTK only lays out and draws new content on its next frame.
  await exports.frame()

  return window
}

exports.frame = function frame() {
  shared()

  return frames.afterAnimationFrame()
}

exports.place = function place(parent, child, frame) {
  child.insertBefore(parent)

  parent.layoutManager.getLayoutChild(child).frame = frame
}

// GTK reports event coordinates as single precision floats.
exports.round = function round(value) {
  return Math.round(value * 1000) / 1000
}
