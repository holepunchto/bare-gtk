const { test } = require('bare-tap')
const Pointer = require('bare-pointer')
const {
  EventControllerFocus,
  EventControllerMotion,
  FrameFixed,
  GestureClick,
  GestureDrag,
  Label,
  constants
} = require('..')
const { mount, place, round } = require('./helpers')

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

async function setup(t, controller) {
  const root = new FrameFixed()
  const box = new FrameFixed()

  box.addController(controller)

  place(root, box, [40, 30, 200, 150])

  await mount(t, root)

  const pointer = new Pointer()

  t.teardown(() => pointer.destroy())

  return { box, pointer }
}

test('reports a click with the pointer', async (t) => {
  const click = new GestureClick()
  const { box, pointer } = await setup(t, click)

  const events = []

  click.on('pressed', (presses, x, y) => events.push(['pressed', presses, round(x), round(y)]))
  click.on('released', (presses, x, y) => events.push(['released', presses, round(x), round(y)]))

  await pointer.tap(box, 10, 20)

  t.deepStrictEqual(events, [
    ['pressed', 1, 10, 20],
    ['released', 1, 10, 20]
  ])
})

test('counts a double click with the pointer', async (t) => {
  const click = new GestureClick()
  const { box, pointer } = await setup(t, click)

  const presses = []

  click.on('pressed', (count) => presses.push(count))

  await pointer.tap(box, 10, 20)
  await pointer.tap(box, 10, 20)

  t.deepStrictEqual(presses, [1, 2])
})

test('reports a drag with the pointer', async (t) => {
  const drag = new GestureDrag()
  const { box, pointer } = await setup(t, drag)

  const events = []

  drag.on('dragBegin', (x, y) => events.push(['dragBegin', round(x), round(y)]))
  drag.on('dragUpdate', (x, y) => events.push(['dragUpdate', round(x), round(y)]))
  drag.on('dragEnd', (x, y) => events.push(['dragEnd', round(x), round(y)]))

  await pointer.down(box, 10, 10)
  await pointer.move(box, 50, 40)
  await pointer.move(box, 90, 70)
  await pointer.up()

  t.deepStrictEqual(events.slice(0, 1), [['dragBegin', 10, 10]], 'began')
  t.deepStrictEqual(
    events.slice(-2),
    [
      ['dragUpdate', 80, 60],
      ['dragEnd', 80, 60]
    ],
    'ended at the offset'
  )
})

test('reports motion with the pointer', async (t) => {
  const motion = new EventControllerMotion()
  const { box, pointer } = await setup(t, motion)

  const events = []

  motion.on('enter', (x, y) => events.push(['enter', round(x), round(y)]))
  motion.on('motion', (x, y) => events.push(['motion', round(x), round(y)]))

  // The cursor stays where the last test left it, so it is moved outside the
  // box first to have it enter.
  await pointer.tap(box, -20, -20)
  await pointer.down(box, 10, 10)
  await pointer.move(box, 50, 40)
  await pointer.up()

  t.ok(
    events.some(([name]) => name === 'enter'),
    'entered'
  )
  t.deepStrictEqual(
    events.filter(([name]) => name === 'motion').slice(-1),
    [['motion', 50, 40]],
    'moved to the point'
  )
})
