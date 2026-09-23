#ifndef BARE_GTK_DISPLAY_H
#define BARE_GTK_DISPLAY_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_display_get_default(js_env_t *env, js_callback_info_t *info) {
  int err;

  return bare_gtk__create_tag(env, gdk_display_get_default());
}

#endif // BARE_GTK_DISPLAY_H
