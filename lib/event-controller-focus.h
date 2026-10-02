#ifndef BARE_GTK_EVENT_CONTROLLER_FOCUS_H
#define BARE_GTK_EVENT_CONTROLLER_FOCUS_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"
#include "event-controller.h"

enum {
  bare_gtk_event_controller_focus_event_enter = 1 << 0,
  bare_gtk_event_controller_focus_event_leave = 1 << 1,
};

static void
bare_gtk_event_controller_focus__on_enter(GtkEventControllerFocus *focus, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, focus) & bare_gtk_event_controller_focus_event_enter) == 0) return;

  bare_gtk__emit(state, focus, "enter", 0, NULL);
}

static void
bare_gtk_event_controller_focus__on_leave(GtkEventControllerFocus *focus, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, focus) & bare_gtk_event_controller_focus_event_leave) == 0) return;

  bare_gtk__emit(state, focus, "leave", 0, NULL);
}

static js_value_t *
bare_gtk_event_controller_focus_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  GtkEventController *focus = gtk_event_controller_focus_new();

  g_signal_connect(focus, "enter", G_CALLBACK(bare_gtk_event_controller_focus__on_enter), state);
  g_signal_connect(focus, "leave", G_CALLBACK(bare_gtk_event_controller_focus__on_leave), state);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject_tag(state->registry, focus), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_EVENT_CONTROLLER_FOCUS_H
