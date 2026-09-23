const binding = require('../binding')

const measurements = new Map()

const MAX_MEASUREMENTS = 8192

exports.measure = function measure(text, style = {}, width = Infinity) {
  const { family = null, size = 0 } = style

  const key = `${family}\u0000${size}\u0000${width}\u0000${text}`

  let measurement = measurements.get(key)

  if (measurement === undefined) {
    const [w, h, lines] = binding.textMeasure(text, family, size, width)

    if (measurements.size >= MAX_MEASUREMENTS) measurements.clear()

    measurements.set(key, (measurement = { width: w, height: h, lines }))
  }

  return measurement
}
