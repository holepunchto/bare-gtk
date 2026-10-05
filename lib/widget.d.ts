import GObject = require('./object')
import GDKCursor = require('./cursor')
import GDKFrameClock = require('./frame-clock')
import GTKEventController = require('./event-controller')
import GTKLayoutManager = require('./layout-manager')

/** The base of every widget, as a `GtkWidget`. */
interface GTKWidget<M extends Record<keyof M, unknown[]> = {}> extends GObject<M> {
  /** Whether the widget is shown. */
  visible: boolean

  /** How opaque the widget is, from 0 to 1. */
  opacity: number

  /** Whether content that does not fit is shown or clipped, as an `OVERFLOW_*` constant. */
  overflow: number

  /** The smallest size the widget asks for, as `[width, height]`. -1 means no request. */
  get sizeRequest(): [width: number, height: number]
  set sizeRequest(size: [width: number, height: number])

  /** How many device pixels the widget has per logical pixel. */
  readonly scaleFactor: number

  /** The nearest widget above this one with a surface of its own, such as a window, or `null`. */
  readonly native: GTKWidget | null

  readonly parent: GTKWidget | null

  readonly firstChild: GTKWidget | null

  readonly lastChild: GTKWidget | null

  readonly nextSibling: GTKWidget | null

  readonly prevSibling: GTKWidget | null

  /** What places the children of the widget, or `null` for the default of its type. */
  layoutManager: GTKLayoutManager | null

  /** The width the widget was given in the last layout. */
  readonly width: number

  /** The height the widget was given in the last layout. */
  readonly height: number

  /** Whether the widget can be the target of pointer events. */
  canTarget: boolean

  /** The cursor shown over the widget, or `null` for the cursor of its parent. */
  cursor: GDKCursor | null

  /** The clock the widget draws by, or `null` before it is realized. */
  readonly frameClock: GDKFrameClock | null

  /** Whether the widget responds to input. */
  sensitive: boolean

  /** Move the keyboard focus to the widget. Returns whether it took it. */
  grabFocus(): boolean

  /** Set which widget in this root has the focus, or `null` to take it away from all of them. */
  setRootFocus(widget: GTKWidget | null): this

  addCssClass(name: string): this

  removeCssClass(name: string): this

  hasCssClass(name: string): boolean

  /** Show the cursor called `name`, such as `'pointer'` or `'text'`. */
  setCursorFromName(name: string): this

  /**
   * Ask how big the widget wants to be along `orientation`, an `ORIENTATION_*` constant, given
   * `forSize` along the other axis. -1, the default, means any size.
   */
  measure(
    orientation: number,
    forSize?: number
  ): [minimum: number, natural: number, minimumBaseline: number, naturalBaseline: number]

  /** The bounds of the widget in the coordinates of `target`, or `null` if they are unrelated. */
  computeBounds(target: GTKWidget): [x: number, y: number, width: number, height: number] | null

  /** Add the widget to `parent`, after `previousSibling`, or first when that is `null`. */
  insertAfter(parent: GTKWidget, previousSibling?: GTKWidget | null): this

  /** Add the widget to `parent`, before `nextSibling`, or last when that is `null`. */
  insertBefore(parent: GTKWidget, nextSibling?: GTKWidget | null): this

  /** Add a controller that reports input on the widget. Returns `controller`. */
  addController<C extends GTKEventController<any>>(controller: C): C

  /** Remove a controller added with `addController()`. Returns `controller`. */
  removeController<C extends GTKEventController<any>>(controller: C): C

  /** Remove the widget from its parent. */
  unparent(): this
}

declare class GTKWidget<M extends Record<keyof M, unknown[]> = {}> {
  protected constructor()
}

export = GTKWidget
