#include <assert.h>
#include <bare.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "lib/widget.h"
#include "lib/window.h"

static js_value_t *
bare_gtk_exports(js_env_t *env, js_value_t *exports) {
  int err;

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, NULL, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("widgetVisible", bare_gtk_widget_visible)

  V("windowInit", bare_gtk_window_init)
  V("windowChild", bare_gtk_window_child)
#undef V

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_int32(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }
#undef V

  return exports;
}

BARE_MODULE(bare_gtk, bare_gtk_exports)
