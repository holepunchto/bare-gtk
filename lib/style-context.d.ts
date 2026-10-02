import GDKDisplay = require('./display')
import GTKCssProvider = require('./css-provider')

/**
 * Apply a style sheet to every widget on `display`. `priority` is a `STYLE_PROVIDER_PRIORITY_*`
 * constant.
 */
export function addProviderForDisplay(
  display: GDKDisplay,
  provider: GTKCssProvider,
  priority: number
): void

/** Stop applying a style sheet added with `addProviderForDisplay()`. */
export function removeProviderForDisplay(display: GDKDisplay, provider: GTKCssProvider): void
