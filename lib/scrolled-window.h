#ifndef BARE_GTK_SCROLLED_WINDOW_H
#define BARE_GTK_SCROLLED_WINDOW_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_scrolled_window_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  GtkWidget *window = gtk_scrolled_window_new();

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(window), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_scrolled_window_child(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkScrolledWindow *window;
  err = bare_gobject__read_tag(env, argv[0], "window", (gpointer *) &window);
  if (err < 0) return NULL;

  if (argc == 1) {
    GtkWidget *child = gtk_scrolled_window_get_child(window);

    js_value_t *result;

    if (child == NULL) {
      err = js_get_null(env, &result);
      assert(err == 0);
    } else {
      err = js_create_uint32(env, bare_gobject__tag(child), &result);
      assert(err == 0);
    }

    return result;
  }

  js_value_type_t type;
  err = js_typeof(env, argv[1], &type);
  assert(err == 0);

  if (type == js_null) {
    gtk_scrolled_window_set_child(window, NULL);

    return NULL;
  }

  GtkWidget *child;
  err = bare_gobject__read_tag(env, argv[1], "child", (gpointer *) &child);
  if (err < 0) return NULL;

  gtk_scrolled_window_set_child(window, child);

  return NULL;
}

static js_value_t *
bare_gtk_scrolled_window_policy(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  GtkScrolledWindow *window;
  err = bare_gobject__read_tag(env, argv[0], "window", (gpointer *) &window);
  if (err < 0) return NULL;

  uint32_t horizontal, vertical;

  err = bare_gtk__read_uint32(env, argv[1], "horizontal", &horizontal);
  if (err < 0) return NULL;

  err = bare_gtk__read_uint32(env, argv[2], "vertical", &vertical);
  if (err < 0) return NULL;

  gtk_scrolled_window_set_policy(window, horizontal, vertical);

  return NULL;
}

// The adjustments are what scrolling actually moves, so they are handed over
// rather than hidden behind a position on the window.
static js_value_t *
bare_gtk_scrolled_window_adjustment(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  GtkScrolledWindow *window;
  err = bare_gobject__read_tag(env, argv[0], "window", (gpointer *) &window);
  if (err < 0) return NULL;

  bool horizontal;
  err = bare_gtk__read_bool(env, argv[1], "horizontal", &horizontal);
  if (err < 0) return NULL;

  GtkAdjustment *adjustment = horizontal
    ? gtk_scrolled_window_get_hadjustment(window)
    : gtk_scrolled_window_get_vadjustment(window);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(adjustment), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_SCROLLED_WINDOW_H
