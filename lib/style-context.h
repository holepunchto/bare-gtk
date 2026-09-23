#ifndef BARE_GTK_STYLE_CONTEXT_H
#define BARE_GTK_STYLE_CONTEXT_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_style_context_add_provider_for_display(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  GdkDisplay *display;
  err = bare_gobject__read_tag(env, argv[0], "display", (gpointer *) &display);
  if (err < 0) return NULL;

  GtkStyleProvider *provider;
  err = bare_gobject__read_tag(env, argv[1], "provider", (gpointer *) &provider);
  if (err < 0) return NULL;

  int32_t priority;
  err = bare_gtk__read_int32(env, argv[2], "priority", &priority);
  if (err < 0) return NULL;

  gtk_style_context_add_provider_for_display(display, provider, priority);

  return NULL;
}

static js_value_t *
bare_gtk_style_context_remove_provider_for_display(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  GdkDisplay *display;
  err = bare_gobject__read_tag(env, argv[0], "display", (gpointer *) &display);
  if (err < 0) return NULL;

  GtkStyleProvider *provider;
  err = bare_gobject__read_tag(env, argv[1], "provider", (gpointer *) &provider);
  if (err < 0) return NULL;

  gtk_style_context_remove_provider_for_display(display, provider);

  return NULL;
}

#endif // BARE_GTK_STYLE_CONTEXT_H
