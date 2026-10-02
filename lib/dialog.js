const binding = require('../binding')
const wrap = require('./wrap')
const GTKWidget = require('./widget')

module.exports = exports = class GTKDialog extends GTKWidget {
  static _events = {
    response: binding.DIALOG_EVENT_RESPONSE
  }

  _eventMask(mask) {
    binding.dialogEventMask(this._tag, mask)
  }

  addButton(label, response) {
    return wrap(GTKWidget, binding.dialogAddButton(this._tag, label, response))
  }

  response(response) {
    binding.dialogResponse(this._tag, response)

    return this
  }
}

exports.RESPONSE = {
  NONE: binding.DIALOG_RESPONSE_NONE,
  REJECT: binding.DIALOG_RESPONSE_REJECT,
  ACCEPT: binding.DIALOG_RESPONSE_ACCEPT,
  DELETE_EVENT: binding.DIALOG_RESPONSE_DELETE_EVENT,
  OK: binding.DIALOG_RESPONSE_OK,
  CANCEL: binding.DIALOG_RESPONSE_CANCEL,
  CLOSE: binding.DIALOG_RESPONSE_CLOSE,
  YES: binding.DIALOG_RESPONSE_YES,
  NO: binding.DIALOG_RESPONSE_NO,
  APPLY: binding.DIALOG_RESPONSE_APPLY,
  HELP: binding.DIALOG_RESPONSE_HELP
}
