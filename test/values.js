const { test } = require('bare-tap')
const { CssProvider, Display, Label, StyleContext, Texture, constants } = require('..')
const Cursor = require('../lib/cursor')
const Settings = require('../lib/settings')
const text = require('../lib/text')

test('creates a cursor by name', (t) => {
  const fallback = Cursor.newFromName('default')
  const cursor = Cursor.newFromName('pointer', fallback)

  t.equal(cursor.name, 'pointer', 'name')
  t.ok(cursor.fallback === fallback, 'same fallback wrapper')
  t.equal(cursor.texture, null, 'no texture')
})

test('fails to read a missing image', (t) => {
  t.throws(() => Texture.newFromFilename('/no/such/file.png'))
})

test('applies a style sheet to the display', (t) => {
  const display = Display.getDefault()
  const provider = new CssProvider()
  const label = new Label()

  provider.loadFromData('label.wide { padding: 0 50px; }')

  label.text = 'A'
  label.addCssClass('wide')

  const [, before] = label.measure(constants.ORIENTATION_HORIZONTAL)

  StyleContext.addProviderForDisplay(display, provider, constants.STYLE_PROVIDER_PRIORITY_USER)

  t.teardown(() => StyleContext.removeProviderForDisplay(display, provider))

  const [, after] = label.measure(constants.ORIENTATION_HORIZONTAL)

  t.equal(after, before + 100)
})

test('round trips text through the clipboard', async (t) => {
  const clipboard = Display.getDefault().getClipboard()

  t.ok(Display.getDefault().getClipboard() === clipboard, 'same wrapper')

  clipboard.setText('bare-gtk')

  t.ok(clipboard.formats.includes('text/plain'), 'holds text')

  const read = await new Promise((resolve, reject) => {
    clipboard.readTextAsync((err, text) => (err ? reject(new Error(err)) : resolve(text)))
  })

  t.equal(read, 'bare-gtk')
})

test('has a separate primary clipboard', (t) => {
  const display = Display.getDefault()

  t.ok(display.getPrimaryClipboard() !== display.getClipboard())
})

test('reads the desktop settings', (t) => {
  const settings = Settings.getDefault()

  t.ok(Settings.getDefault() === settings, 'same wrapper')
  t.equal(typeof settings.preferDarkTheme, 'boolean', 'a theme preference')
  t.equal(typeof settings.xftDpi, 'number', 'a font resolution')
})

test('measures text', (t) => {
  const short = text.measure('Hello')
  const long = text.measure('Hello, world')

  t.ok(short.width > 0 && short.height > 0, 'has a size')
  t.equal(short.lines, 1, 'one line')
  t.ok(long.width > short.width, 'wider when longer')

  const wrapped = text.measure('Hello, world', {}, long.width / 2)

  t.ok(wrapped.lines > 1, 'wraps')
  t.ok(text.measure('Hello', { size: 40 }).height > short.height, 'taller when larger')
  t.ok(text.measureMarkup('<big>Hello</big>').width > short.width, 'with markup')
})
