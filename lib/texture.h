#ifndef BARE_GTK_TEXTURE_H
#define BARE_GTK_TEXTURE_H

#include <assert.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "bridging.h"

// A texture arrives owned rather than floating, unlike a widget, so the
// reference this call holds is handed to the registry and dropped here.
static js_value_t *
bare_gtk_texture_new_from_filename(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  char *path;
  err = bare_gtk__read_string(env, argv[0], "path", &path);
  if (err < 0) return NULL;

  GError *error = NULL;

  GdkTexture *texture = gdk_texture_new_from_filename(path, &error);

  g_free(path);

  if (texture == NULL) {
    bare_gtk__throw(env, error);

    return NULL;
  }

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(texture), &result);
  assert(err == 0);

  g_object_unref(texture);

  return result;
}

static js_value_t *
bare_gtk_texture_width(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GdkTexture *texture;
  err = bare_gobject__read_tag(env, argv[0], "texture", (gpointer *) &texture);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_create_int32(env, gdk_texture_get_width(texture), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_texture_height(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GdkTexture *texture;
  err = bare_gobject__read_tag(env, argv[0], "texture", (gpointer *) &texture);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_create_int32(env, gdk_texture_get_height(texture), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_TEXTURE_H
