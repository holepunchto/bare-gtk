import GTKWidget = require('./widget')
import GTKTextBuffer = require('./text-buffer')

/** Several lines of editable text, as a `GtkTextView`. */
interface GTKTextView extends GTKWidget<GTKTextView.Events> {
  /** The text the view shows. */
  readonly buffer: GTKTextBuffer

  /** Whether the user can change the text. */
  editable: boolean

  /** How lines wrap, as a `WRAP_*` constant. */
  wrapMode: number

  /** What the text is for, as a `GTKEntry.INPUT_PURPOSE` constant. */
  inputPurpose: number

  /** Hints for input methods, as `GTKEntry.INPUT_HINTS` flags combined with `|`. */
  inputHints: number

  /** How lines line up with each other, as a `JUSTIFY_*` constant. */
  justification: number
}

declare class GTKTextView {
  constructor()
}

declare namespace GTKTextView {
  export interface Events {
    /** The view got or lost the keyboard. `focused` is 1 or 0. */
    hasFocus: [focused: number]

    /** The view became or stopped being the focus widget of its window. `focused` is 1 or 0. */
    isFocus: [focused: number]
  }
}

export = GTKTextView
