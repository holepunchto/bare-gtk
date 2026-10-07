const { test } = require('bare-tap')
const {
  Entry,
  EventControllerFocus,
  Fixed,
  Label,
  Spinner,
  Switch,
  TextView,
  constants
} = require('..')
const { frame, mount } = require('./helpers')

test('sets the text of a label', (t) => {
  const label = new Label()

  t.equal(label.text, '', 'empty')

  label.text = 'Grüße 👋'

  t.equal(label.text, 'Grüße 👋', 'round trips non-ASCII text')

  label.setMarkup('<b>Bold</b>')

  t.equal(label.text, 'Bold', 'markup is not part of the text')
})

test('sets how a label wraps and aligns', (t) => {
  const label = new Label()

  label.wrap = true
  label.wrapMode = constants.WRAP_CHAR
  label.justify = constants.JUSTIFY_CENTER
  label.ellipsize = constants.ELLIPSIZE_END
  label.lines = 2
  label.xalign = 0
  label.yalign = 1

  t.equal(label.wrap, true, 'wrap')
  t.equal(label.wrapMode, constants.WRAP_CHAR, 'wrap mode')
  t.equal(label.justify, constants.JUSTIFY_CENTER, 'justify')
  t.equal(label.ellipsize, constants.ELLIPSIZE_END, 'ellipsize')
  t.equal(label.lines, 2, 'lines')
  t.equal(label.xalign, 0, 'x alignment')
  t.equal(label.yalign, 1, 'y alignment')
})

test('grows a label with its text', (t) => {
  const label = new Label()

  label.text = 'A'

  const [, short] = label.measure(constants.ORIENTATION_HORIZONTAL)

  label.text = 'A much longer string'

  const [, long] = label.measure(constants.ORIENTATION_HORIZONTAL)

  t.ok(long > short)
})

test('emits when the text of an entry changes', (t) => {
  const entry = new Entry()

  const events = []

  entry.on('changed', () => events.push('changed'))
  entry.on('insert-text', (text, start) => events.push(['insert-text', text, start]))

  entry.text = 'Hello'

  t.equal(entry.text, 'Hello', 'text')
  t.ok(events.includes('changed'), 'changed')
  t.deepStrictEqual(
    events.find((event) => event[0] === 'insert-text'),
    ['insert-text', 'Hello', 0],
    'inserted'
  )
})

test('stops reporting once nothing listens', (t) => {
  const entry = new Entry()

  let changes = 0

  const onchanged = () => changes++

  entry.on('changed', onchanged)
  entry.text = 'a'
  entry.off('changed', onchanged)
  entry.text = 'b'

  t.equal(changes, 1)
})

test('selects a region of an entry', async (t) => {
  const entry = new Entry()

  await mount(t, entry)

  entry.text = 'Hello'
  entry.grabFocusWithoutSelecting()
  entry.selectRegion(1, 3)

  t.deepStrictEqual(entry.selectionBounds, { start: 1, end: 3 }, 'selected')

  entry.selectRegion(2, 2)

  t.deepStrictEqual(entry.selectionBounds, { start: 2, end: 2 }, 'a caret')

  entry.selectRegion(0, -1)

  t.deepStrictEqual(entry.selectionBounds, { start: 0, end: 5 }, 'to the end')
})

test('sets the hints of an entry', (t) => {
  const entry = new Entry()

  t.equal(entry.editable, true, 'editable')
  t.equal(entry.visibility, true, 'visible text')
  t.equal(entry.placeholderText, null, 'no placeholder')
  t.equal(entry.inputPurpose, Entry.INPUT_PURPOSE.FREE_FORM, 'free form')

  entry.editable = false
  entry.visibility = false
  entry.placeholderText = 'Email'
  entry.inputPurpose = Entry.INPUT_PURPOSE.EMAIL
  entry.inputHints = Entry.INPUT_HINTS.NO_SPELLCHECK | Entry.INPUT_HINTS.LOWERCASE
  entry.alignment = 1

  t.equal(entry.editable, false, 'not editable')
  t.equal(entry.visibility, false, 'hidden text')
  t.equal(entry.placeholderText, 'Email', 'placeholder')
  t.equal(entry.inputPurpose, Entry.INPUT_PURPOSE.EMAIL, 'purpose')
  t.equal(entry.inputHints, Entry.INPUT_HINTS.NO_SPELLCHECK | Entry.INPUT_HINTS.LOWERCASE, 'hints')
  t.equal(entry.alignment, 1, 'alignment')

  entry.placeholderText = null

  // GTK keeps an empty placeholder once one has been set.
  t.equal(entry.placeholderText, '', 'placeholder cleared')
})

test('reports focus moving between entries', async (t) => {
  const parent = new Fixed()
  const a = new Entry()
  const b = new Entry()

  a.insertBefore(parent)
  b.insertBefore(parent)

  const events = []

  // An entry hands the focus to the text inside it, so it is never the focus
  // widget itself. The controller is added before the entries are shown, as a
  // window that lost its focus widget gives the focus to the first one.
  const focus = a.addController(new EventControllerFocus())

  focus.on('enter', () => events.push('enter'))
  focus.on('leave', () => events.push('leave'))

  await mount(t, parent)

  t.equal(a.grabFocus(), true, 'took the focus')

  await frame()

  b.grabFocus()

  await frame()

  t.deepStrictEqual(events, ['enter', 'leave'])
})

test('emits when a switch is toggled', (t) => {
  const toggle = new Switch()

  let changes = 0

  toggle.on('active', () => changes++)

  t.equal(toggle.active, false, 'off')

  toggle.active = true

  t.equal(toggle.active, true, 'on')
  t.equal(changes, 1, 'emitted for a change from code')

  toggle.active = true

  t.equal(changes, 1, 'not for the same value')
})

test('spins a spinner', (t) => {
  const spinner = new Spinner()

  t.equal(spinner.spinning, false, 'still')

  spinner.spinning = true

  t.equal(spinner.spinning, true, 'spinning')
})

test('edits the buffer of a text view', (t) => {
  const view = new TextView()
  const buffer = view.buffer

  t.ok(view.buffer === buffer, 'same buffer wrapper')

  let changes = 0

  buffer.on('changed', () => changes++)
  buffer.text = 'Hello\nWorld'

  t.equal(buffer.text, 'Hello\nWorld', 'text')
  t.equal(changes, 1, 'changed')

  buffer.selectRange(2, 8)

  t.deepStrictEqual(buffer.selectionBounds, { start: 2, end: 8 }, 'selected')
})

test('sets the hints of a text view', (t) => {
  const view = new TextView()

  t.equal(view.editable, true, 'editable')

  view.editable = false
  view.wrapMode = constants.WRAP_WORD
  view.justification = constants.JUSTIFY_RIGHT
  view.inputPurpose = Entry.INPUT_PURPOSE.URL
  view.inputHints = Entry.INPUT_HINTS.NO_SPELLCHECK

  t.equal(view.editable, false, 'not editable')
  t.equal(view.wrapMode, constants.WRAP_WORD, 'wrap mode')
  t.equal(view.justification, constants.JUSTIFY_RIGHT, 'justification')
  t.equal(view.inputPurpose, Entry.INPUT_PURPOSE.URL, 'purpose')
  t.equal(view.inputHints, Entry.INPUT_HINTS.NO_SPELLCHECK, 'hints')
})
