#ifndef BARE_GTK_EDITABLE_H
#define BARE_GTK_EDITABLE_H

#include <assert.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "bridging.h"

// `GtkEditable` is an interface, so what it contributes is the text itself and
// what can be done to it, which an entry implements and a text view does not.
static js_value_t *
bare_gtk_editable_text(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkEditable *editable;
  err = bare_gobject__read_tag(env, argv[0], "editable", (gpointer *) &editable);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    const char *text = gtk_editable_get_text(editable);

    err = js_create_string_utf8(env, (const utf8_t *) (text == NULL ? "" : text), (size_t) -1, &result);
    assert(err == 0);
  } else {
    char *text;
    err = bare_gtk__read_string(env, argv[1], "text", &text);
    if (err < 0) return NULL;

    gtk_editable_set_text(editable, text);

    g_free(text);
  }

  return result;
}

static js_value_t *
bare_gtk_editable_editable(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkEditable *editable;
  err = bare_gobject__read_tag(env, argv[0], "editable", (gpointer *) &editable);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, gtk_editable_get_editable(editable), &result);
    assert(err == 0);
  } else {
    bool value;
    err = bare_gtk__read_bool(env, argv[1], "editable", &value);
    if (err < 0) return NULL;

    gtk_editable_set_editable(editable, value);
  }

  return result;
}

// The bounds are two out parameters and a boolean saying whether anything is
// selected at all; with nothing selected the caret is where both ends are.
static js_value_t *
bare_gtk_editable_selection_bounds(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GtkEditable *editable;
  err = bare_gobject__read_tag(env, argv[0], "editable", (gpointer *) &editable);
  if (err < 0) return NULL;

  int start = 0;
  int end = 0;

  if (!gtk_editable_get_selection_bounds(editable, &start, &end)) {
    start = end = gtk_editable_get_position(editable);
  }

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

  js_value_t *value;

  err = js_create_int32(env, start, &value);
  assert(err == 0);

  err = js_set_named_property(env, result, "start", value);
  assert(err == 0);

  err = js_create_int32(env, end, &value);
  assert(err == 0);

  err = js_set_named_property(env, result, "end", value);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_editable_select_region(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  GtkEditable *editable;
  err = bare_gobject__read_tag(env, argv[0], "editable", (gpointer *) &editable);
  if (err < 0) return NULL;

  int32_t start;
  err = bare_gtk__read_int32(env, argv[1], "start", &start);
  if (err < 0) return NULL;

  int32_t end;
  err = bare_gtk__read_int32(env, argv[2], "end", &end);
  if (err < 0) return NULL;

  gtk_editable_select_region(editable, start, end);

  return NULL;
}

#endif // BARE_GTK_EDITABLE_H
