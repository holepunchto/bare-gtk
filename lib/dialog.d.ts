import GTKWidget = require('./widget')

/** A dialog window, as a `GtkDialog`. */
interface GTKDialog<M extends Record<keyof M, unknown[]> = GTKDialog.Events> extends GTKWidget<M> {
  /**
   * Add a button that closes the dialog with `response`. Use 0 and up for your own buttons; the
   * negative values are the `RESPONSE` constants. Returns the button.
   */
  addButton(label: string, response: number): GTKWidget

  /** Close the dialog with `response`, as if a button with that response was pressed. */
  response(response: number): this
}

declare class GTKDialog<M extends Record<keyof M, unknown[]> = GTKDialog.Events> {
  protected constructor()

  /** The responses GTK defines. They are all negative. */
  static readonly RESPONSE: {
    readonly NONE: number
    readonly REJECT: number
    readonly ACCEPT: number
    readonly DELETE_EVENT: number
    readonly OK: number
    readonly CANCEL: number
    readonly CLOSE: number
    readonly YES: number
    readonly NO: number
    readonly APPLY: number
    readonly HELP: number
  }
}

declare namespace GTKDialog {
  export interface Events {
    /** A button was pressed or the dialog was closed. */
    response: [response: number]
  }
}

export = GTKDialog
