import GTKWidget = require('./widget')

/**
 * A single line of editable text, as a `GtkEntry`. The focus goes to the text inside the entry
 * rather than the entry itself, so follow it with a `GTKEventControllerFocus`.
 */
interface GTKEntry extends GTKWidget<GTKEntry.Events> {
  text: string

  /** Whether the user can change the text. */
  editable: boolean

  /** The selected range, as character offsets. With nothing selected, both are at the caret. */
  readonly selectionBounds: { start: number; end: number }

  /** Select the characters from `start` to `end`. -1 means the end of the text. */
  selectRegion(start: number, end: number): this

  /** Take the keyboard focus without selecting all the text, as `grabFocus()` does. */
  grabFocusWithoutSelecting(): boolean

  /** Text shown while the entry is empty, or `null`. */
  placeholderText: string | null

  /** Whether the text is shown. Set it to `false` for a password. */
  visibility: boolean

  /** What the text is for, as an `INPUT_PURPOSE` constant. */
  inputPurpose: number

  /** Hints for input methods, as `INPUT_HINTS` flags combined with `|`. */
  inputHints: number

  /** Where the text sits when it is shorter than the entry, from 0 (start) to 1 (end). */
  alignment: number
}

declare class GTKEntry {
  constructor()

  static readonly INPUT_PURPOSE: {
    readonly FREE_FORM: 0
    readonly ALPHA: 1
    readonly DIGITS: 2
    readonly NUMBER: 3
    readonly PHONE: 4
    readonly URL: 5
    readonly EMAIL: 6
    readonly NAME: 7
    readonly PASSWORD: 8
    readonly PIN: 9
    readonly TERMINAL: 10
  }

  static readonly INPUT_HINTS: {
    readonly NONE: number
    readonly SPELLCHECK: number
    readonly NO_SPELLCHECK: number
    readonly WORD_COMPLETION: number
    readonly LOWERCASE: number
    readonly UPPERCASE_CHARS: number
    readonly UPPERCASE_WORDS: number
    readonly UPPERCASE_SENTENCES: number
    readonly INHIBIT_OSK: number
  }
}

declare namespace GTKEntry {
  export interface Events {
    /** The text changed. */
    changed: []

    /** The user pressed Enter. */
    activate: []

    /** The caret moved. */
    cursorPosition: []

    /** Text is about to be inserted at `start`. `end` is the same as `start`. */
    insertText: [text: string, start: number, end: number]

    /** The text from `start` to `end` is about to be deleted. `text` is empty. */
    deleteText: [text: string, start: number, end: number]
  }
}

export = GTKEntry
