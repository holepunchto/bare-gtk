#ifndef BARE_GTK_SURFACE_H
#define BARE_GTK_SURFACE_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>
#endif

#include "bridging.h"

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
    GdkSurface *surface; \
    err = bare_gobject_read_tag(env, state->registry, argv[0], "surface", (gpointer *) &surface); \
    if (err < 0) return NULL; \
\
    js_value_t *result; \
    err = js_create_int32(env, fn(surface), &result); \
    assert(err == 0); \
\
    return result; \
  }

V(bare_gtk_surface_width, gdk_surface_get_width)
V(bare_gtk_surface_height, gdk_surface_get_height)
V(bare_gtk_surface_scale_factor, gdk_surface_get_scale_factor)
#undef V

static js_value_t *
bare_gtk_surface_display(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkSurface *surface;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "surface", (gpointer *) &surface);
  if (err < 0) return NULL;

  return bare_gtk__create_tag(env, state, gdk_surface_get_display(surface));
}

static js_value_t *
bare_gtk_surface_xid(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkSurface *surface;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "surface", (gpointer *) &surface);
  if (err < 0) return NULL;

  js_value_t *result;

#ifdef GDK_WINDOWING_X11
  if (GDK_IS_X11_SURFACE(surface)) {
    err = js_create_uint32(env, (uint32_t) gdk_x11_surface_get_xid(surface), &result);
    assert(err == 0);

    return result;
  }
#endif

  err = js_get_null(env, &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_SURFACE_H
