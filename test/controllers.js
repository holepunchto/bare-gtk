const { test } = require('bare-tap')
const {
  EventControllerFocus,
  EventControllerMotion,
  GestureClick,
  Label,
  constants
} = require('..')

test('adds and removes a controller', (t) => {
  const label = new Label()
  const click = new GestureClick()

  t.ok(label.addController(click) === click, 'returns the controller')
  t.ok(label.removeController(click) === click, 'and again')
})

test('sets the button of a gesture', (t) => {
  const click = new GestureClick()

  t.equal(click.button, 1, 'the primary button')

  click.button = 0

  t.equal(click.button, 0, 'any button')
})

test('sets the propagation phase', (t) => {
  const motion = new EventControllerMotion()

  t.equal(motion.propagationPhase, constants.PROPAGATION_PHASE_BUBBLE, 'bubble')

  motion.propagationPhase = constants.PROPAGATION_PHASE_CAPTURE

  t.equal(motion.propagationPhase, constants.PROPAGATION_PHASE_CAPTURE, 'capture')
})

test('has no device outside an event', (t) => {
  const focus = new EventControllerFocus()

  t.equal(focus.currentEventDevice, null)
})
