import GObject = require('./object')

/** A value within a range, as a `GtkAdjustment`. Scrolled windows scroll by moving one. */
interface GTKAdjustment extends GObject<GTKAdjustment.Events> {
  value: number

  /** The largest value. */
  readonly upper: number
}

declare class GTKAdjustment {
  protected constructor()
}

declare namespace GTKAdjustment {
  export interface Events {
    valueChanged: [value: number]
  }
}

export = GTKAdjustment
