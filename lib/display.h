#ifndef BARE_GTK_DISPLAY_H
#define BARE_GTK_DISPLAY_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_display_get_default(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  return bare_gtk__create_tag(env, state, gdk_display_get_default());
}

static js_value_t *
bare_gtk_display_get_clipboard(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkDisplay *display;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "display", (gpointer *) &display);
  if (err < 0) return NULL;

  return bare_gtk__create_tag(env, state, gdk_display_get_clipboard(display));
}

static js_value_t *
bare_gtk_display_get_primary_clipboard(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkDisplay *display;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "display", (gpointer *) &display);
  if (err < 0) return NULL;

  return bare_gtk__create_tag(env, state, gdk_display_get_primary_clipboard(display));
}

static js_value_t *
bare_gtk_display_get_monitor_at_surface(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GdkDisplay *display;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "display", (gpointer *) &display);
  if (err < 0) return NULL;

  GdkSurface *surface;
  err = bare_gobject_read_tag(env, state->registry, argv[1], "surface", (gpointer *) &surface);
  if (err < 0) return NULL;

  return bare_gtk__create_tag(env, state, gdk_display_get_monitor_at_surface(display, surface));
}

#endif // BARE_GTK_DISPLAY_H
