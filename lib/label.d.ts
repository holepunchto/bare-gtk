import GTKWidget = require('./widget')

/** A widget that shows text, as a `GtkLabel`. */
interface GTKLabel extends GTKWidget {
  text: string

  /** Show Pango markup, such as `'<b>bold</b>'`. */
  setMarkup(markup: string): this

  /** Whether long lines wrap. */
  wrap: boolean

  /** How lines wrap, as a `WRAP_*` constant. */
  wrapMode: number

  /** Where the text sits horizontally, from 0 (left) to 1 (right). */
  xalign: number

  /** Where the text sits vertically, from 0 (top) to 1 (bottom). */
  yalign: number

  /** How lines line up with each other, as a `JUSTIFY_*` constant. */
  justify: number

  /** The most lines to show when `ellipsize` is set. -1 means no limit. */
  lines: number

  /** Where text that does not fit is cut off with "...", as an `ELLIPSIZE_*` constant. */
  ellipsize: number
}

declare class GTKLabel {
  constructor()
}

export = GTKLabel
