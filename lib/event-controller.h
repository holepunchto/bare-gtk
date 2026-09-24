#ifndef BARE_GTK_EVENT_CONTROLLER_H
#define BARE_GTK_EVENT_CONTROLLER_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static const char *bare_gtk_controller__events = "bare-gtk-events";
static const char *bare_gtk_controller__env = "bare-gtk-env";

static uint32_t
bare_gtk_controller__observed(gpointer controller) {
  return GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(controller), bare_gtk_controller__events));
}

static js_env_t *
bare_gtk_controller__js(gpointer controller) {
  return g_object_get_data(G_OBJECT(controller), bare_gtk_controller__env);
}

static js_value_t *
bare_gtk_event_controller_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  GtkEventController *controller;
  err = bare_gobject__read_tag(env, argv[0], "controller", (gpointer *) &controller);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  g_object_set_data(G_OBJECT(controller), bare_gtk_controller__events, GUINT_TO_POINTER(events));

  return NULL;
}

static js_value_t *
bare_gtk_event_controller_propagation_phase(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkEventController *controller;
  err = bare_gobject__read_tag(env, argv[0], "controller", (gpointer *) &controller);
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

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GtkEventController *controller;
  err = bare_gobject__read_tag(env, argv[0], "controller", (gpointer *) &controller);
  if (err < 0) return NULL;

  GdkDevice *device = gtk_event_controller_get_current_event_device(controller);

  js_value_t *result;

  if (device == NULL) {
    err = js_get_null(env, &result);
    assert(err == 0);
  } else {
    err = js_create_uint32(env, bare_gobject__tag(device), &result);
    assert(err == 0);
  }

  return result;
}

#endif // BARE_GTK_EVENT_CONTROLLER_H
