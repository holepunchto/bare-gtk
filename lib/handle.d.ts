import { Wrapper } from 'bare-gobject-registry'

/** Return the tag of `object` in this addon, adopting it from another addon first if needed. */
export function adopt(object: Wrapper): number
export function adopt(object: null | undefined): null
