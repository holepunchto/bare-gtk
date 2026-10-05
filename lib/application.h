#ifndef BARE_GTK_APPLICATION_H
#define BARE_GTK_APPLICATION_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_application_get_default(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  return bare_gtk__create_tag(env, state, g_application_get_default());
}

static js_value_t *
bare_gtk_application_hold(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GApplication *application;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "application", (gpointer *) &application);
  if (err < 0) return NULL;

  g_application_hold(application);

  return NULL;
}

static js_value_t *
bare_gtk_application_release(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GApplication *application;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "application", (gpointer *) &application);
  if (err < 0) return NULL;

  g_application_release(application);

  return NULL;
}

// The window that was most recently active, which is the most recently added
// one until any of them has been focused.
static js_value_t *
bare_gtk_application_active_window(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GtkApplication *application;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "application", (gpointer *) &application);
  if (err < 0) return NULL;

  return bare_gtk__create_tag(env, state, gtk_application_get_active_window(application));
}

#endif // BARE_GTK_APPLICATION_H
