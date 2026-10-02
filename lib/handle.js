const binding = require('../binding')
const registry = require('bare-gobject-registry')

exports.adopt = function adopt(object) {
  return registry.adopt(binding, object)
}
