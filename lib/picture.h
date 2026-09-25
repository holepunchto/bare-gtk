#ifndef BARE_GTK_PICTURE_H
#define BARE_GTK_PICTURE_H

#include <assert.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_picture_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  GtkWidget *picture = gtk_picture_new();

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(picture), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_picture_paintable(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkPicture *picture;
  err = bare_gobject__read_tag(env, argv[0], "picture", (gpointer *) &picture);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    result = bare_gtk__create_tag(env, gtk_picture_get_paintable(picture));
  } else {
    GdkPaintable *paintable;
    err = bare_gtk__read_tag_or_null(env, argv[1], "paintable", (gpointer *) &paintable);
    if (err < 0) return NULL;

    gtk_picture_set_paintable(picture, paintable);
  }

  return result;
}

static js_value_t *
bare_gtk_picture_keep_aspect_ratio(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkPicture *picture;
  err = bare_gobject__read_tag(env, argv[0], "picture", (gpointer *) &picture);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, gtk_picture_get_keep_aspect_ratio(picture), &result);
    assert(err == 0);
  } else {
    bool keep;
    err = bare_gtk__read_bool(env, argv[1], "keepAspectRatio", &keep);
    if (err < 0) return NULL;

    gtk_picture_set_keep_aspect_ratio(picture, keep);
  }

  return result;
}

static js_value_t *
bare_gtk_picture_can_shrink(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkPicture *picture;
  err = bare_gobject__read_tag(env, argv[0], "picture", (gpointer *) &picture);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, gtk_picture_get_can_shrink(picture), &result);
    assert(err == 0);
  } else {
    bool shrink;
    err = bare_gtk__read_bool(env, argv[1], "canShrink", &shrink);
    if (err < 0) return NULL;

    gtk_picture_set_can_shrink(picture, shrink);
  }

  return result;
}

#endif // BARE_GTK_PICTURE_H
