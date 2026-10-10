const binding = require('../binding')
const wrap = require('./wrap')
const GObject = require('./object')

module.exports = exports = class GTKSettings extends GObject {
  static _events = {
    preferDarkTheme: binding.SETTINGS_EVENT_PREFER_DARK_THEME,
    xftDpi: binding.SETTINGS_EVENT_XFT_DPI
  }

  _eventMask(mask) {
    binding.settingsEventMask(this._tag, mask)
  }

  static getDefault() {
    return wrap(GTKSettings, binding.settingsGetDefault())
  }

  get preferDarkTheme() {
    return binding.settingsPreferDarkTheme(this._tag)
  }

  set preferDarkTheme(value) {
    binding.settingsPreferDarkTheme(this._tag, value)
  }

  get xftDpi() {
    return binding.settingsXftDpi(this._tag)
  }

  set xftDpi(value) {
    binding.settingsXftDpi(this._tag, value)
  }
}
