import GTKDialog = require('./dialog')
import GTKWindow = require('./window')

/** A dialog that shows a message, as a `GtkMessageDialog`. Add its buttons with `addButton()`. */
interface GTKMessageDialog extends GTKDialog {
  /** Text shown below the main message. */
  set secondaryText(text: string)
}

declare class GTKMessageDialog {
  /**
   * Create a message dialog. `messageType` is a `MESSAGE_TYPE` constant and defaults to
   * `MESSAGE_TYPE.OTHER`.
   */
  constructor(opts?: { parent?: GTKWindow | null; messageType?: number; text?: string })

  static readonly MESSAGE_TYPE: {
    readonly INFO: number
    readonly WARNING: number
    readonly QUESTION: number
    readonly ERROR: number
    readonly OTHER: number
  }
}

export = GTKMessageDialog
