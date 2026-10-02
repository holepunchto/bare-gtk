#ifndef BARE_GTK_MONITOR_H
#define BARE_GTK_MONITOR_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_monitor_geometry(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkMonitor *monitor;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "monitor", (gpointer *) &monitor);
  if (err < 0) return NULL;

  GdkRectangle geometry;
  gdk_monitor_get_geometry(monitor, &geometry);

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

  const char *names[] = {"x", "y", "width", "height"};
  int values[] = {geometry.x, geometry.y, geometry.width, geometry.height};

  for (size_t i = 0; i < 4; i++) {
    js_value_t *value;
    err = js_create_int32(env, values[i], &value);
    assert(err == 0);

    err = js_set_named_property(env, result, names[i], value);
    assert(err == 0);
  }

  return result;
}

#define V(name, fn) \
  static js_value_t * \
  name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 1; \
    js_value_t *argv[1]; \
\
    bare_gtk_state_t *state; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state); \
    assert(err == 0); \
\
    assert(argc == 1); \
\
    GdkMonitor *monitor; \
    err = bare_gobject_read_tag(env, state->registry, argv[0], "monitor", (gpointer *) &monitor); \
    if (err < 0) return NULL; \
\
    js_value_t *result; \
    err = js_create_int32(env, fn(monitor), &result); \
    assert(err == 0); \
\
    return result; \
  }

V(bare_gtk_monitor_scale_factor, gdk_monitor_get_scale_factor)
V(bare_gtk_monitor_width_mm, gdk_monitor_get_width_mm)
V(bare_gtk_monitor_height_mm, gdk_monitor_get_height_mm)
V(bare_gtk_monitor_refresh_rate, gdk_monitor_get_refresh_rate)
#undef V

#define V(name, fn) \
  static js_value_t * \
  name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 1; \
    js_value_t *argv[1]; \
\
    bare_gtk_state_t *state; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state); \
    assert(err == 0); \
\
    assert(argc == 1); \
\
    GdkMonitor *monitor; \
    err = bare_gobject_read_tag(env, state->registry, argv[0], "monitor", (gpointer *) &monitor); \
    if (err < 0) return NULL; \
\
    const char *value = fn(monitor); \
\
    js_value_t *result; \
\
    if (value == NULL) { \
      err = js_get_null(env, &result); \
    } else { \
      err = js_create_string_utf8(env, (const utf8_t *) value, -1, &result); \
    } \
\
    assert(err == 0); \
\
    return result; \
  }

V(bare_gtk_monitor_connector, gdk_monitor_get_connector)
V(bare_gtk_monitor_manufacturer, gdk_monitor_get_manufacturer)
V(bare_gtk_monitor_model, gdk_monitor_get_model)
#undef V

static js_value_t *
bare_gtk_monitor_valid(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkMonitor *monitor;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "monitor", (gpointer *) &monitor);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_get_boolean(env, gdk_monitor_is_valid(monitor), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_MONITOR_H
