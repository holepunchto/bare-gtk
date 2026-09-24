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
  if ((bare_gtk_controller__observed(motion) & bare_gtk_event_controller_motion_event_motion) == 0) return;

  bare_gtk__emit(bare_gtk_controller__js(motion), motion, "motion", 2, (const double[]) {x, y});
}

static void
bare_gtk_event_controller_motion__on_enter(GtkEventControllerMotion *motion, double x, double y, gpointer data) {
  if ((bare_gtk_controller__observed(motion) & bare_gtk_event_controller_motion_event_enter) == 0) return;

  bare_gtk__emit(bare_gtk_controller__js(motion), motion, "enter", 2, (const double[]) {x, y});
}

static void
bare_gtk_event_controller_motion__on_leave(GtkEventControllerMotion *motion, gpointer data) {
  if ((bare_gtk_controller__observed(motion) & bare_gtk_event_controller_motion_event_leave) == 0) return;

  bare_gtk__emit(bare_gtk_controller__js(motion), motion, "leave", 0, NULL);
}

static js_value_t *
bare_gtk_event_controller_motion_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  GtkEventController *motion = gtk_event_controller_motion_new();

  g_object_set_data(G_OBJECT(motion), bare_gtk_controller__env, env);

  g_signal_connect(motion, "motion", G_CALLBACK(bare_gtk_event_controller_motion__on_motion), NULL);
  g_signal_connect(motion, "enter", G_CALLBACK(bare_gtk_event_controller_motion__on_enter), NULL);
  g_signal_connect(motion, "leave", G_CALLBACK(bare_gtk_event_controller_motion__on_leave), NULL);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(motion), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_EVENT_CONTROLLER_MOTION_H
