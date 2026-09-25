const GObject = require('./object')

// `GdkPaintable` is an interface, so what it contributes is the type a caller
// can be handed: a texture is one, and so is anything else a picture is given.
module.exports = exports = class GDKPaintable extends GObject {}
