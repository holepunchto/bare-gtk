#ifndef BARE_GTK_GESTURE_H
#define BARE_GTK_GESTURE_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_gesture_set_state(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  GtkGesture *gesture;
  err = bare_gobject__read_tag(env, argv[0], "gesture", (gpointer *) &gesture);
  if (err < 0) return NULL;

  uint32_t state;
  err = bare_gtk__read_uint32(env, argv[1], "state", &state);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_get_boolean(env, gtk_gesture_set_state(gesture, state), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_gesture_single_button(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkGestureSingle *gesture;
  err = bare_gobject__read_tag(env, argv[0], "gesture", (gpointer *) &gesture);
  if (err < 0) return NULL;

  if (argc == 1) {
    js_value_t *result;
    err = js_create_uint32(env, gtk_gesture_single_get_button(gesture), &result);
    assert(err == 0);

    return result;
  }

  uint32_t button;
  err = bare_gtk__read_uint32(env, argv[1], "button", &button);
  if (err < 0) return NULL;

  gtk_gesture_single_set_button(gesture, button);

  return NULL;
}

#endif // BARE_GTK_GESTURE_H
