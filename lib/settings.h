#ifndef BARE_GTK_SETTINGS_H
#define BARE_GTK_SETTINGS_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

enum {
  bare_gtk_settings_event_prefer_dark_theme = 1 << 0,
  bare_gtk_settings_event_xft_dpi = 1 << 1,
};

static void
bare_gtk_settings__on_prefer_dark_theme(GObject *object, GParamSpec *spec, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, object) & bare_gtk_settings_event_prefer_dark_theme) == 0) return;

  bare_gtk__emit(state, object, "prefer-dark-theme", 0, NULL);
}

static void
bare_gtk_settings__on_xft_dpi(GObject *object, GParamSpec *spec, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, object) & bare_gtk_settings_event_xft_dpi) == 0) return;

  bare_gtk__emit(state, object, "xft-dpi", 0, NULL);
}

static js_value_t *
bare_gtk_settings_get_default(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  GtkSettings *settings = gtk_settings_get_default();

  return bare_gtk__create_tag(env, state, settings);
}

// Connected once and left connected, so the mask alone decides whether anything
// is sent. Settings belong to the display and outlive every wrapper.
static js_value_t *
bare_gtk_settings_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GtkSettings *settings;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "settings", (gpointer *) &settings);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  if (bare_gtk__observe(state, settings, events)) {
    g_signal_connect(
      settings,
      "notify::gtk-application-prefer-dark-theme",
      G_CALLBACK(bare_gtk_settings__on_prefer_dark_theme),
      state
    );

    g_signal_connect(
      settings,
      "notify::gtk-xft-dpi",
      G_CALLBACK(bare_gtk_settings__on_xft_dpi),
      state
    );
  }

  return NULL;
}

static js_value_t *
bare_gtk_settings_prefer_dark_theme(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkSettings *settings;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "settings", (gpointer *) &settings);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    gboolean prefer_dark;

    g_object_get(settings, "gtk-application-prefer-dark-theme", &prefer_dark, NULL);

    err = js_get_boolean(env, prefer_dark, &result);
    assert(err == 0);
  } else {
    bool prefer_dark;
    err = bare_gtk__read_bool(env, argv[1], "prefer_dark_theme", &prefer_dark);
    if (err < 0) return NULL;

    g_object_set(settings, "gtk-application-prefer-dark-theme", (gboolean) prefer_dark, NULL);
  }

  return result;
}

static js_value_t *
bare_gtk_settings_xft_dpi(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkSettings *settings;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "settings", (gpointer *) &settings);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    int dpi;

    g_object_get(settings, "gtk-xft-dpi", &dpi, NULL);

    err = js_create_int32(env, dpi, &result);
    assert(err == 0);
  } else {
    int32_t dpi;
    err = bare_gtk__read_int32(env, argv[1], "xft_dpi", &dpi);
    if (err < 0) return NULL;

    g_object_set(settings, "gtk-xft-dpi", (int) dpi, NULL);
  }

  return result;
}

#endif // BARE_GTK_SETTINGS_H
