module.exports = exports = function observe(target) {
  const events = target.constructor._events

  if (events === undefined) return

  let mask = 0

  target
    .on('newListener', (name) => {
      const bit = events[name]

      if (bit === undefined || target.listenerCount(name) !== 0) return

      target._eventMask((mask |= bit))
    })
    .on('removeListener', (name) => {
      const bit = events[name]

      if (bit === undefined || target.listenerCount(name) !== 0) return

      target._eventMask((mask &= ~bit))
    })
}
