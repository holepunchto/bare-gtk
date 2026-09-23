#ifndef BARE_GTK_FIXED_H
#define BARE_GTK_FIXED_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_fixed_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  GtkWidget *fixed = gtk_fixed_new();

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject_tag(state->registry, fixed), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_FIXED_H
