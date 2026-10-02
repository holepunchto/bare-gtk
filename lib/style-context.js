const binding = require('../binding')

exports.addProviderForDisplay = function addProviderForDisplay(display, provider, priority) {
  binding.styleContextAddProviderForDisplay(display._tag, provider._tag, priority)
}

exports.removeProviderForDisplay = function removeProviderForDisplay(display, provider) {
  binding.styleContextRemoveProviderForDisplay(display._tag, provider._tag)
}
