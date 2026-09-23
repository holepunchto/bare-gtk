import GObject = require('./object')

/** An input device, as a `GdkDevice`. */
interface GTKDevice extends GObject {
  /** What kind of device it is, as an `INPUT_SOURCE_*` constant. */
  readonly source: number
}

declare class GTKDevice {
  protected constructor()
}

export = GTKDevice
