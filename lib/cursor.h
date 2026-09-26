#ifndef BARE_GTK_CURSOR_H
#define BARE_GTK_CURSOR_H

#include <assert.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "bridging.h"

// A cursor arrives owned rather than floating, as a texture does, so the
// reference this call holds is handed to the registry and dropped here.
static js_value_t *
bare_gtk_cursor_new_from_name(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  char *name;
  err = bare_gtk__read_string(env, argv[0], "name", &name);
  if (err < 0) return NULL;

  GdkCursor *fallback;
  err = bare_gtk__read_tag_or_null(env, argv[1], "fallback", (gpointer *) &fallback);
  if (err < 0) {
    g_free(name);

    return NULL;
  }

  GdkCursor *cursor = gdk_cursor_new_from_name(name, fallback);

  g_free(name);

  js_value_t *result;

  if (cursor == NULL) {
    err = js_get_null(env, &result);
    assert(err == 0);

    return result;
  }

  err = js_create_uint32(env, bare_gobject__tag(cursor), &result);
  assert(err == 0);

  g_object_unref(cursor);

  return result;
}

static js_value_t *
bare_gtk_cursor_new_from_texture(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 4);

  GdkTexture *texture;
  err = bare_gobject__read_tag(env, argv[0], "texture", (gpointer *) &texture);
  if (err < 0) return NULL;

  int32_t hotspot_x;
  err = bare_gtk__read_int32(env, argv[1], "hotspot_x", &hotspot_x);
  if (err < 0) return NULL;

  int32_t hotspot_y;
  err = bare_gtk__read_int32(env, argv[2], "hotspot_y", &hotspot_y);
  if (err < 0) return NULL;

  GdkCursor *fallback;
  err = bare_gtk__read_tag_or_null(env, argv[3], "fallback", (gpointer *) &fallback);
  if (err < 0) return NULL;

  GdkCursor *cursor = gdk_cursor_new_from_texture(texture, hotspot_x, hotspot_y, fallback);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(cursor), &result);
  assert(err == 0);

  g_object_unref(cursor);

  return result;
}

static js_value_t *
bare_gtk_cursor_name(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GdkCursor *cursor;
  err = bare_gobject__read_tag(env, argv[0], "cursor", (gpointer *) &cursor);
  if (err < 0) return NULL;

  const char *name = gdk_cursor_get_name(cursor);

  js_value_t *result;

  if (name == NULL) {
    err = js_get_null(env, &result);
    assert(err == 0);
  } else {
    err = js_create_string_utf8(env, (const utf8_t *) name, (size_t) -1, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_gtk_cursor_fallback(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GdkCursor *cursor;
  err = bare_gobject__read_tag(env, argv[0], "cursor", (gpointer *) &cursor);
  if (err < 0) return NULL;

  return bare_gtk__create_tag(env, gdk_cursor_get_fallback(cursor));
}

static js_value_t *
bare_gtk_cursor_texture(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GdkCursor *cursor;
  err = bare_gobject__read_tag(env, argv[0], "cursor", (gpointer *) &cursor);
  if (err < 0) return NULL;

  return bare_gtk__create_tag(env, gdk_cursor_get_texture(cursor));
}

static js_value_t *
bare_gtk_cursor_hotspot_x(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GdkCursor *cursor;
  err = bare_gobject__read_tag(env, argv[0], "cursor", (gpointer *) &cursor);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_create_int32(env, gdk_cursor_get_hotspot_x(cursor), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_cursor_hotspot_y(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GdkCursor *cursor;
  err = bare_gobject__read_tag(env, argv[0], "cursor", (gpointer *) &cursor);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_create_int32(env, gdk_cursor_get_hotspot_y(cursor), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_CURSOR_H
