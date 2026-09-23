const binding = require('../binding')

const cache = new Map()

const CACHE_LIMIT = 4096

exports.measure = function measure(text, style = {}, width = Infinity) {
  const { family = null, size = 0 } = style

  const key = `${family}\u0000${size}\u0000${width}\u0000${text}`

  let result = cache.get(key)

  if (result === undefined) {
    const [w, h, lines] = binding.textMeasure(text, family, size, width)

    if (cache.size === CACHE_LIMIT) cache.delete(cache.keys().next().value)

    cache.set(key, (result = { width: w, height: h, lines }))
  }

  return result
}
