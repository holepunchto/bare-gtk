exports.retain = function retain(owner, wrapper) {
  if (wrapper !== null) owner._retained.add(wrapper)

  return wrapper
}

exports.release = function release(owner, wrapper) {
  if (wrapper !== null) owner._retained.delete(wrapper)

  return wrapper
}
