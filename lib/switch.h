#ifndef BARE_GTK_SWITCH_H
#define BARE_GTK_SWITCH_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

enum {
  bare_gtk_switch_event_active = 1 << 0,
};

// `GtkSwitch` cannot be subclassed, so the handler is kept in the data table of
// the object and the signal carries the state.
#define BARE_GTK_SWITCH_ACTIVE "bare.switch.active"

// This fires whoever changed the property, including the program itself. A
// controlled switch has to tell those apart in the layers above.
static void
bare_gtk_switch__on_active(GObject *object, GParamSpec *spec, gpointer data) {
  bare_gtk__emit(data, object, "active", 0, NULL);
}

static js_value_t *
bare_gtk_switch_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  GtkWidget *handle = gtk_switch_new();

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject_tag(state->registry, handle), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_switch_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GtkSwitch *handle;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "switch", (gpointer *) &handle);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  gulong id = (gulong) (uintptr_t) g_object_get_data(G_OBJECT(handle), BARE_GTK_SWITCH_ACTIVE);

  if ((events & bare_gtk_switch_event_active) != 0) {
    if (id == 0) {
      id = g_signal_connect(handle, "notify::active", G_CALLBACK(bare_gtk_switch__on_active), state);

      g_object_set_data(G_OBJECT(handle), BARE_GTK_SWITCH_ACTIVE, (gpointer) (uintptr_t) id);
    }
  } else if (id != 0) {
    g_signal_handler_disconnect(handle, id);

    g_object_set_data(G_OBJECT(handle), BARE_GTK_SWITCH_ACTIVE, NULL);
  }

  return NULL;
}

static js_value_t *
bare_gtk_switch_active(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkSwitch *handle;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "switch", (gpointer *) &handle);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, gtk_switch_get_active(handle), &result);
    assert(err == 0);
  } else {
    bool active;
    err = bare_gtk__read_bool(env, argv[1], "active", &active);
    if (err < 0) return NULL;

    gtk_switch_set_active(handle, active);
  }

  return result;
}

#endif // BARE_GTK_SWITCH_H
