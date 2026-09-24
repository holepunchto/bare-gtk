#ifndef BARE_GTK_VIEWPORT_H
#define BARE_GTK_VIEWPORT_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_viewport_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  GtkWidget *viewport = gtk_viewport_new(NULL, NULL);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(viewport), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_viewport_child(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkViewport *viewport;
  err = bare_gobject__read_tag(env, argv[0], "viewport", (gpointer *) &viewport);
  if (err < 0) return NULL;

  if (argc == 1) {
    GtkWidget *child = gtk_viewport_get_child(viewport);

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
    gtk_viewport_set_child(viewport, NULL);

    return NULL;
  }

  GtkWidget *child;
  err = bare_gobject__read_tag(env, argv[1], "child", (gpointer *) &child);
  if (err < 0) return NULL;

  gtk_viewport_set_child(viewport, child);

  return NULL;
}

// A viewport is a `GtkScrollable`, and the policy is what decides whether the
// child is given its minimum size along an axis or the size it asked for. A
// minimum of zero, which a layout that is placed rather than measured reports,
// leaves nothing to scroll under the default.
static js_value_t *
bare_gtk_viewport_scroll_policy(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  GtkViewport *viewport;
  err = bare_gobject__read_tag(env, argv[0], "viewport", (gpointer *) &viewport);
  if (err < 0) return NULL;

  uint32_t horizontal, vertical;

  err = bare_gtk__read_uint32(env, argv[1], "horizontal", &horizontal);
  if (err < 0) return NULL;

  err = bare_gtk__read_uint32(env, argv[2], "vertical", &vertical);
  if (err < 0) return NULL;

  gtk_scrollable_set_hscroll_policy(GTK_SCROLLABLE(viewport), horizontal);
  gtk_scrollable_set_vscroll_policy(GTK_SCROLLABLE(viewport), vertical);

  return NULL;
}

#endif // BARE_GTK_VIEWPORT_H
