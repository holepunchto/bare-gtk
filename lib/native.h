#ifndef BARE_GTK_NATIVE_H
#define BARE_GTK_NATIVE_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_native_surface(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GtkNative *native;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "native", (gpointer *) &native);
  if (err < 0) return NULL;

  return bare_gtk__create_tag(env, state, gtk_native_get_surface(native));
}

#endif // BARE_GTK_NATIVE_H
