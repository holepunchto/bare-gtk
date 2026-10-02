#ifndef BARE_GTK_VIEWPORT_H
#define BARE_GTK_VIEWPORT_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_viewport_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  GtkWidget *viewport = gtk_viewport_new(NULL, NULL);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject_tag(state->registry, viewport), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_viewport_child(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkViewport *viewport;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "viewport", (gpointer *) &viewport);
  if (err < 0) return NULL;

  if (argc == 1) {
    GtkWidget *child = gtk_viewport_get_child(viewport);

    js_value_t *result;

    if (child == NULL) {
      err = js_get_null(env, &result);
      assert(err == 0);
    } else {
      err = js_create_uint32(env, bare_gobject_tag(state->registry, child), &result);
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
  err = bare_gobject_read_tag(env, state->registry, argv[1], "child", (gpointer *) &child);
  if (err < 0) return NULL;

  gtk_viewport_set_child(viewport, child);

  return NULL;
}

// The scroll policy decides whether the child gets its minimum size along an
// axis or the size it asked for. A child that reports a minimum of zero, as a
// placed layout does, has nothing to scroll by default.
static js_value_t *
bare_gtk_viewport_hscroll_policy(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkViewport *viewport;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "viewport", (gpointer *) &viewport);
  if (err < 0) return NULL;

  if (argc == 1) {
    js_value_t *result;
    err = js_create_uint32(env, gtk_scrollable_get_hscroll_policy(GTK_SCROLLABLE(viewport)), &result);
    assert(err == 0);

    return result;
  }

  uint32_t policy;
  err = bare_gtk__read_uint32(env, argv[1], "policy", &policy);
  if (err < 0) return NULL;

  gtk_scrollable_set_hscroll_policy(GTK_SCROLLABLE(viewport), policy);

  return NULL;
}

static js_value_t *
bare_gtk_viewport_vscroll_policy(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkViewport *viewport;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "viewport", (gpointer *) &viewport);
  if (err < 0) return NULL;

  if (argc == 1) {
    js_value_t *result;
    err = js_create_uint32(env, gtk_scrollable_get_vscroll_policy(GTK_SCROLLABLE(viewport)), &result);
    assert(err == 0);

    return result;
  }

  uint32_t policy;
  err = bare_gtk__read_uint32(env, argv[1], "policy", &policy);
  if (err < 0) return NULL;

  gtk_scrollable_set_vscroll_policy(GTK_SCROLLABLE(viewport), policy);

  return NULL;
}

#endif // BARE_GTK_VIEWPORT_H
