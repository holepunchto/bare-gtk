# bare-gtk

GTK 4 for Bare on Linux. It gives you GTK widgets in JavaScript, along with a runtime that starts GTK for you, so a Bare app can have a native Linux window.

```
npm i bare-gtk
```

## Usage

```js
const { Window, Label, GestureClick } = require('bare-gtk')

const window = new Window()

window.title = 'Hello'
window.defaultSize = [400, 300]

const label = new Label()

label.text = 'Click me'

const click = label.addController(new GestureClick())

let clicks = 0

click.on('pressed', () => {
  label.text = `Clicked ${++clicks} times`
})

window.child = label
window.visible = true
```

Build the app with `bare-build` and this runtime. GTK needs an application ID, which `--identifier` sets:

```console
bare-build --host linux-x64 --runtime bare-gtk/runtime --identifier com.example.Hello index.js
```

Objects that have events, such as windows, entries and gestures, emit them as ordinary events. GTK is only asked to report an event while something listens to it.

## License

Apache-2.0
