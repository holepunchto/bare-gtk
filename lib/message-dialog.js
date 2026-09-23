const binding = require('../binding')
const GTKDialog = require('./dialog')

module.exports = exports = class GTKMessageDialog extends GTKDialog {
  _init(opts) {
    const { parent = null, messageType = exports.MESSAGE_TYPE.OTHER, text = '' } = opts

    return binding.messageDialogNew(parent === null ? null : parent._tag, messageType, text)
  }

  set secondaryText(text) {
    binding.messageDialogSecondaryText(this._tag, text)
  }
}

exports.MESSAGE_TYPE = {
  INFO: binding.MESSAGE_DIALOG_MESSAGE_TYPE_INFO,
  WARNING: binding.MESSAGE_DIALOG_MESSAGE_TYPE_WARNING,
  QUESTION: binding.MESSAGE_DIALOG_MESSAGE_TYPE_QUESTION,
  ERROR: binding.MESSAGE_DIALOG_MESSAGE_TYPE_ERROR,
  OTHER: binding.MESSAGE_DIALOG_MESSAGE_TYPE_OTHER
}
