#ifndef BARE_GTK_LABEL_H
#define BARE_GTK_LABEL_H

#include <assert.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_label_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  GtkWidget *label = gtk_label_new(NULL);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(label), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_label_text(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkLabel *label;
  err = bare_gobject__read_tag(env, argv[0], "label", (gpointer *) &label);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    const char *text = gtk_label_get_text(label);

    err = js_create_string_utf8(env, (const utf8_t *) text, (size_t) -1, &result);
    assert(err == 0);
  } else {
    char *text;
    err = bare_gtk__read_string(env, argv[1], "text", &text);
    if (err < 0) return NULL;

    gtk_label_set_text(label, text);

    g_free(text);
  }

  return result;
}

static js_value_t *
bare_gtk_label_wrap(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkLabel *label;
  err = bare_gobject__read_tag(env, argv[0], "label", (gpointer *) &label);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, gtk_label_get_wrap(label), &result);
    assert(err == 0);
  } else {
    bool wrap;
    err = bare_gtk__read_bool(env, argv[1], "wrap", &wrap);
    if (err < 0) return NULL;

    gtk_label_set_wrap(label, wrap);
  }

  return result;
}

static js_value_t *
bare_gtk_label_wrap_mode(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkLabel *label;
  err = bare_gobject__read_tag(env, argv[0], "label", (gpointer *) &label);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_create_int32(env, gtk_label_get_wrap_mode(label), &result);
    assert(err == 0);
  } else {
    int32_t mode;
    err = bare_gtk__read_int32(env, argv[1], "wrapMode", &mode);
    if (err < 0) return NULL;

    gtk_label_set_wrap_mode(label, mode);
  }

  return result;
}

#define V(name, get, set) \
  static js_value_t * \
  name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 2; \
    js_value_t *argv[2]; \
\
    err = js_get_callback_info(env, info, &argc, argv, NULL, NULL); \
    assert(err == 0); \
\
    assert(argc == 1 || argc == 2); \
\
    GtkLabel *label; \
    err = bare_gobject__read_tag(env, argv[0], "label", (gpointer *) &label); \
    if (err < 0) return NULL; \
\
    js_value_t *result = NULL; \
\
    if (argc == 1) { \
      err = js_create_double(env, get(label), &result); \
      assert(err == 0); \
    } else { \
      double align; \
      err = bare_gtk__read_double(env, argv[1], "align", &align); \
      if (err < 0) return NULL; \
\
      set(label, (float) align); \
    } \
\
    return result; \
  }

V(bare_gtk_label_xalign, gtk_label_get_xalign, gtk_label_set_xalign)
V(bare_gtk_label_yalign, gtk_label_get_yalign, gtk_label_set_yalign)
#undef V

static js_value_t *
bare_gtk_label_justify(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkLabel *label;
  err = bare_gobject__read_tag(env, argv[0], "label", (gpointer *) &label);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_create_int32(env, gtk_label_get_justify(label), &result);
    assert(err == 0);
  } else {
    int32_t justify;
    err = bare_gtk__read_int32(env, argv[1], "justify", &justify);
    if (err < 0) return NULL;

    gtk_label_set_justify(label, justify);
  }

  return result;
}

static js_value_t *
bare_gtk_label_set_markup(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  GtkLabel *label;
  err = bare_gobject__read_tag(env, argv[0], "label", (gpointer *) &label);
  if (err < 0) return NULL;

  char *markup;
  err = bare_gtk__read_string(env, argv[1], "markup", &markup);
  if (err < 0) return NULL;

  gtk_label_set_markup(label, markup);

  g_free(markup);

  return NULL;
}

#define V(name, get, set, type) \
  static js_value_t * \
  name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 2; \
    js_value_t *argv[2]; \
\
    err = js_get_callback_info(env, info, &argc, argv, NULL, NULL); \
    assert(err == 0); \
\
    assert(argc == 1 || argc == 2); \
\
    GtkLabel *label; \
    err = bare_gobject__read_tag(env, argv[0], "label", (gpointer *) &label); \
    if (err < 0) return NULL; \
\
    js_value_t *result = NULL; \
\
    if (argc == 1) { \
      err = js_create_int32(env, get(label), &result); \
      assert(err == 0); \
    } else { \
      int32_t value; \
      err = bare_gtk__read_int32(env, argv[1], #name, &value); \
      if (err < 0) return NULL; \
\
      set(label, (type) value); \
    } \
\
    return result; \
  }

V(bare_gtk_label_lines, gtk_label_get_lines, gtk_label_set_lines, int)
V(bare_gtk_label_ellipsize, gtk_label_get_ellipsize, gtk_label_set_ellipsize, PangoEllipsizeMode)
#undef V

#endif // BARE_GTK_LABEL_H
