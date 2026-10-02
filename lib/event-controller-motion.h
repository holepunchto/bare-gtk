#ifndef BARE_GTK_EVENT_CONTROLLER_MOTION_H
#define BARE_GTK_EVENT_CONTROLLER_MOTION_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"
#include "event-controller.h"

enum {
  bare_gtk_event_controller_motion_event_motion = 1 << 0,
  bare_gtk_event_controller_motion_event_enter = 1 << 1,
  bare_gtk_event_controller_motion_event_leave = 1 << 2,
};

static void
bare_gtk_event_controller_motion__on_motion(GtkEventControllerMotion *motion, double x, double y, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, motion) & bare_gtk_event_controller_motion_event_motion) == 0) return;

  bare_gtk__emit(state, motion, "motion", 2, (const double[]) {x, y});
}

static void
bare_gtk_event_controller_motion__on_enter(GtkEventControllerMotion *motion, double x, double y, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, motion) & bare_gtk_event_controller_motion_event_enter) == 0) return;

  bare_gtk__emit(state, motion, "enter", 2, (const double[]) {x, y});
}

static void
bare_gtk_event_controller_motion__on_leave(GtkEventControllerMotion *motion, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, motion) & bare_gtk_event_controller_motion_event_leave) == 0) return;

  bare_gtk__emit(state, motion, "leave", 0, NULL);
}

static js_value_t *
bare_gtk_event_controller_motion_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  GtkEventController *motion = gtk_event_controller_motion_new();

  g_signal_connect(motion, "motion", G_CALLBACK(bare_gtk_event_controller_motion__on_motion), state);
  g_signal_connect(motion, "enter", G_CALLBACK(bare_gtk_event_controller_motion__on_enter), state);
  g_signal_connect(motion, "leave", G_CALLBACK(bare_gtk_event_controller_motion__on_leave), state);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject_tag(state->registry, motion), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_EVENT_CONTROLLER_MOTION_H
