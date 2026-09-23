const binding = require('../binding')
const registry = require('bare-gobject-registry')

exports.handle = registry.handle

exports.adopt = function adopt(object) {
  return registry.adopt(binding, object)
}

exports.expose = function expose(classes) {
  return registry.expose(binding, classes)
}
