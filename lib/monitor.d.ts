import GObject = require('./object')

/** A monitor, as a `GdkMonitor`. */
interface GDKMonitor extends GObject {
  /** Where the monitor is and how big it is, in logical pixels. */
  readonly geometry: { x: number; y: number; width: number; height: number }

  readonly scaleFactor: number

  readonly widthMm: number

  readonly heightMm: number

  /** The refresh rate in millihertz, or 0 if it is not known. */
  readonly refreshRate: number

  /** The name of the connector, such as `'HDMI-1'`, or `null`. */
  readonly connector: string | null

  readonly manufacturer: string | null

  readonly model: string | null

  /** Whether the monitor is still connected. */
  readonly valid: boolean
}

declare class GDKMonitor {
  protected constructor()
}

export = GDKMonitor
