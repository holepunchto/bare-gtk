#include <assert.h>
#include <bare.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "lib/bridging.h"
#include "lib/css-provider.h"
#include "lib/display.h"
#include "lib/fixed.h"
#include "lib/frame-layout.h"
#include "lib/label.h"
#include "lib/layout-child.h"
#include "lib/layout-manager.h"
#include "lib/registry.h"
#include "lib/style-context.h"
#include "lib/text.h"
#include "lib/widget.h"
#include "lib/window.h"

static js_value_t *
bare_gtk_init_check(js_env_t *env, js_callback_info_t *info) {
  int err;

  js_value_t *result;
  err = js_get_boolean(env, gtk_init_check(), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_exports(js_env_t *env, js_value_t *exports) {
  int err;

  // Constructing a widget before GTK is initialized segfaults. A no-op under the
  // runtime, which has already done it.
  gtk_init_check();

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, NULL, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("initCheck", bare_gtk_init_check)

  V("claim", bare_gobject_registry_claim)
  V("wrapper", bare_gobject_registry_wrapper)
  V("registrySize", bare_gobject_registry_size)
  V("handle", bare_gobject_registry_handle)
  V("adopt", bare_gobject_registry_adopt)

  V("widgetVisible", bare_gtk_widget_visible)
  V("widgetSizeRequest", bare_gtk_widget_size_request)
  V("widgetOpacity", bare_gtk_widget_opacity)
  V("widgetOverflow", bare_gtk_widget_overflow)
  V("widgetParent", bare_gtk_widget_parent)
  V("widgetInsertAfter", bare_gtk_widget_insert_after)
  V("widgetInsertBefore", bare_gtk_widget_insert_before)
  V("widgetUnparent", bare_gtk_widget_unparent)
  V("widgetFirstChild", bare_gtk_widget_first_child)
  V("widgetLastChild", bare_gtk_widget_last_child)
  V("widgetNextSibling", bare_gtk_widget_next_sibling)
  V("widgetPrevSibling", bare_gtk_widget_prev_sibling)
  V("widgetWidth", bare_gtk_widget_width)
  V("widgetHeight", bare_gtk_widget_height)
  V("widgetComputeBounds", bare_gtk_widget_compute_bounds)
  V("widgetMeasure", bare_gtk_widget_measure)
  V("widgetAddCssClass", bare_gtk_widget_add_css_class)
  V("widgetRemoveCssClass", bare_gtk_widget_remove_css_class)
  V("widgetHasCssClass", bare_gtk_widget_has_css_class)

  V("widgetLayoutManager", bare_gtk_widget_layout_manager)

  V("fixedInit", bare_gtk_fixed_init)

  V("layoutManagerLayoutChild", bare_gtk_layout_manager_layout_child)

  V("layoutChildChildWidget", bare_gtk_layout_child_child_widget)

  V("frameLayoutInit", bare_gtk_frame_layout_init)
  V("frameLayoutEventMask", bare_gtk_frame_layout_event_mask)
  V("frameLayoutChildFrame", bare_gtk_frame_layout_child_frame)

  V("labelInit", bare_gtk_label_init)
  V("labelText", bare_gtk_label_text)
  V("labelWrap", bare_gtk_label_wrap)
  V("labelWrapMode", bare_gtk_label_wrap_mode)
  V("labelXalign", bare_gtk_label_xalign)
  V("labelYalign", bare_gtk_label_yalign)
  V("labelJustify", bare_gtk_label_justify)

  V("cssProviderInit", bare_gtk_css_provider_init)
  V("cssProviderLoadFromData", bare_gtk_css_provider_load_from_data)

  V("displayGetDefault", bare_gtk_display_get_default)

  V("styleContextAddProviderForDisplay", bare_gtk_style_context_add_provider_for_display)
  V("styleContextRemoveProviderForDisplay", bare_gtk_style_context_remove_provider_for_display)

  V("textMeasure", bare_gtk_text_measure)

  V("windowInit", bare_gtk_window_init)
  V("windowTitle", bare_gtk_window_title)
  V("windowDefaultSize", bare_gtk_window_default_size)
  V("windowChild", bare_gtk_window_child)
  V("windowEventMask", bare_gtk_window_event_mask)
#undef V

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_int32(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("ORIENTATION_HORIZONTAL", GTK_ORIENTATION_HORIZONTAL)
  V("ORIENTATION_VERTICAL", GTK_ORIENTATION_VERTICAL)

  V("OVERFLOW_VISIBLE", GTK_OVERFLOW_VISIBLE)
  V("OVERFLOW_HIDDEN", GTK_OVERFLOW_HIDDEN)

  V("JUSTIFY_LEFT", GTK_JUSTIFY_LEFT)
  V("JUSTIFY_RIGHT", GTK_JUSTIFY_RIGHT)
  V("JUSTIFY_CENTER", GTK_JUSTIFY_CENTER)
  V("JUSTIFY_FILL", GTK_JUSTIFY_FILL)

  V("STYLE_PROVIDER_PRIORITY_FALLBACK", GTK_STYLE_PROVIDER_PRIORITY_FALLBACK)
  V("STYLE_PROVIDER_PRIORITY_THEME", GTK_STYLE_PROVIDER_PRIORITY_THEME)
  V("STYLE_PROVIDER_PRIORITY_SETTINGS", GTK_STYLE_PROVIDER_PRIORITY_SETTINGS)
  V("STYLE_PROVIDER_PRIORITY_APPLICATION", GTK_STYLE_PROVIDER_PRIORITY_APPLICATION)
  V("STYLE_PROVIDER_PRIORITY_USER", GTK_STYLE_PROVIDER_PRIORITY_USER)

  V("WRAP_WORD", PANGO_WRAP_WORD)
  V("WRAP_CHAR", PANGO_WRAP_CHAR)
  V("WRAP_WORD_CHAR", PANGO_WRAP_WORD_CHAR)

  V("FRAME_LAYOUT_EVENT_RESIZE", bare_gtk_frame_layout_event_resize)

  V("WINDOW_EVENT_CLOSE_REQUEST", bare_gtk_window_event_close_request)
#undef V

  return exports;
}

BARE_MODULE(bare_gtk, bare_gtk_exports)
