#ifndef BARE_GTK_LAYOUT_CHILD_H
#define BARE_GTK_LAYOUT_CHILD_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

#define V(name, fn) \
  static js_value_t * \
  name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 1; \
    js_value_t *argv[1]; \
\
    err = js_get_callback_info(env, info, &argc, argv, NULL, NULL); \
    assert(err == 0); \
\
    assert(argc == 1); \
\
    GtkLayoutChild *layout_child; \
    err = bare_gobject__read_tag(env, argv[0], "layoutChild", (gpointer *) &layout_child); \
    if (err < 0) return NULL; \
\
    return bare_gtk__create_tag(env, fn(layout_child)); \
  }

V(bare_gtk_layout_child_child_widget, gtk_layout_child_get_child_widget)
#undef V

#endif // BARE_GTK_LAYOUT_CHILD_H
