exports.hold = function hold(owner, wrapper) {
  if (wrapper !== null) owner._retained.add(wrapper)

  return wrapper
}

exports.drop = function drop(owner, wrapper) {
  if (wrapper !== null) owner._retained.delete(wrapper)

  return wrapper
}
