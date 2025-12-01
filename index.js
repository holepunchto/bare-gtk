require.asset = require('require-asset')

const prebuilds = {}

exports.prebuilds = prebuilds

prebuilds['linux-x64'] = () => {
  return require.asset('./prebuilds/linux-x64/bare', __filename)
}

prebuilds['linux-arm64'] = () => {
  return require.asset('./prebuilds/linux-arm64/bare', __filename)
}
