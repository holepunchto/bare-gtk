export interface Measurement {
  width: number
  height: number
  lines: number
}

/**
 * Measure `text` as a label would show it, wrapped at `width` pixels, which defaults to
 * `Infinity`. `style.size` is in pixels, and 0 means the default size. Results are cached.
 */
export function measure(
  text: string,
  style?: { family?: string | null; size?: number },
  width?: number
): Measurement

/** Measure Pango markup as a label would show it, wrapped at `width` pixels. */
export function measureMarkup(markup: string, width?: number): Measurement
