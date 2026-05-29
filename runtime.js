const { type } = require('bare-build/constants')

const { EXECUTABLE } = type

require.asset = require('require-asset')

const prebuilds = {}

exports.prebuilds = prebuilds

prebuilds['linux-x64'] = () => {
  return {
    type: EXECUTABLE,
    path: require.asset('./prebuilds/linux-x64/bare', __filename),
    dependencies: []
  }
}

prebuilds['linux-arm64'] = () => {
  return {
    type: EXECUTABLE,
    path: require.asset('./prebuilds/linux-arm64/bare', __filename),
    dependencies: []
  }
}
