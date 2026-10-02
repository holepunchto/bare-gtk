#ifndef BARE_GTK_GESTURE_CLICK_H
#define BARE_GTK_GESTURE_CLICK_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"
#include "event-controller.h"

enum {
  bare_gtk_gesture_click_event_pressed = 1 << 0,
  bare_gtk_gesture_click_event_released = 1 << 1,
  bare_gtk_gesture_click_event_cancel = 1 << 2,
};

static void
bare_gtk_gesture_click__on_pressed(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, gesture) & bare_gtk_gesture_click_event_pressed) == 0) return;

  bare_gtk__emit(state, gesture, "pressed", 3, (const double[]) {n_press, x, y});
}

static void
bare_gtk_gesture_click__on_released(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, gesture) & bare_gtk_gesture_click_event_released) == 0) return;

  bare_gtk__emit(state, gesture, "released", 3, (const double[]) {n_press, x, y});
}

static void
bare_gtk_gesture_click__on_cancel(GtkGesture *gesture, GdkEventSequence *sequence, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, gesture) & bare_gtk_gesture_click_event_cancel) == 0) return;

  bare_gtk__emit(state, gesture, "cancel", 0, NULL);
}

static js_value_t *
bare_gtk_gesture_click_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  GtkGesture *gesture = gtk_gesture_click_new();

  g_signal_connect(gesture, "pressed", G_CALLBACK(bare_gtk_gesture_click__on_pressed), state);
  g_signal_connect(gesture, "released", G_CALLBACK(bare_gtk_gesture_click__on_released), state);
  g_signal_connect(gesture, "cancel", G_CALLBACK(bare_gtk_gesture_click__on_cancel), state);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject_tag(state->registry, gesture), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_GESTURE_CLICK_H
