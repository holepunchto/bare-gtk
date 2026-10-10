import GObject = require('./object')

/** The text of a `GTKTextView`, as a `GtkTextBuffer`. Positions are character offsets. */
interface GTKTextBuffer extends GObject<GTKTextBuffer.Events> {
  text: string

  /** The selected range. With nothing selected, both are at the caret. */
  readonly selectionBounds: { start: number; end: number }

  /** Select the characters from `start` to `end`. */
  selectRange(start: number, end: number): this
}

declare class GTKTextBuffer {
  protected constructor()
}

declare namespace GTKTextBuffer {
  export interface Events {
    changed: []
    cursorPosition: []

    /** Text is about to be inserted at `start`. `end` is the same as `start`. */
    insertText: [text: string, start: number, end: number]

    /** The text from `start` to `end` is about to be deleted. `text` is empty. */
    deleteRange: [text: string, start: number, end: number]
  }
}

export = GTKTextBuffer
