#ifndef BARE_GTK_EVENT_CONTROLLER_H
#define BARE_GTK_EVENT_CONTROLLER_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_event_controller_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GtkEventController *controller;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "controller", (gpointer *) &controller);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  bare_gtk__observe(state, controller, events);

  return NULL;
}

static js_value_t *
bare_gtk_event_controller_propagation_phase(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkEventController *controller;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "controller", (gpointer *) &controller);
  if (err < 0) return NULL;

  if (argc == 1) {
    js_value_t *result;
    err = js_create_uint32(env, gtk_event_controller_get_propagation_phase(controller), &result);
    assert(err == 0);

    return result;
  }

  uint32_t phase;
  err = bare_gtk__read_uint32(env, argv[1], "phase", &phase);
  if (err < 0) return NULL;

  gtk_event_controller_set_propagation_phase(controller, phase);

  return NULL;
}

static js_value_t *
bare_gtk_event_controller_current_event_device(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GtkEventController *controller;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "controller", (gpointer *) &controller);
  if (err < 0) return NULL;

  GdkDevice *device = gtk_event_controller_get_current_event_device(controller);

  js_value_t *result;

  if (device == NULL) {
    err = js_get_null(env, &result);
    assert(err == 0);
  } else {
    err = js_create_uint32(env, bare_gobject_tag(state->registry, device), &result);
    assert(err == 0);
  }

  return result;
}

#endif // BARE_GTK_EVENT_CONTROLLER_H
