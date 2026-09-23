#ifndef BARE_GTK_CSS_PROVIDER_H
#define BARE_GTK_CSS_PROVIDER_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_css_provider_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  GtkCssProvider *provider = gtk_css_provider_new();

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject_tag(state->registry, provider), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_css_provider_load_from_data(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GtkCssProvider *provider;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "cssProvider", (gpointer *) &provider);
  if (err < 0) return NULL;

  char *data;
  err = bare_gtk__read_string(env, argv[1], "data", &data);
  if (err < 0) return NULL;

  gtk_css_provider_load_from_data(provider, data, -1);

  g_free(data);

  return NULL;
}

#endif // BARE_GTK_CSS_PROVIDER_H
