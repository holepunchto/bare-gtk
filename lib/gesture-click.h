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
  if ((bare_gtk_controller__observed(gesture) & bare_gtk_gesture_click_event_pressed) == 0) return;

  bare_gtk__emit(bare_gtk_controller__js(gesture), gesture, "pressed", 3, (const double[]) {n_press, x, y});
}

static void
bare_gtk_gesture_click__on_released(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  if ((bare_gtk_controller__observed(gesture) & bare_gtk_gesture_click_event_released) == 0) return;

  bare_gtk__emit(bare_gtk_controller__js(gesture), gesture, "released", 3, (const double[]) {n_press, x, y});
}

static void
bare_gtk_gesture_click__on_cancel(GtkGesture *gesture, GdkEventSequence *sequence, gpointer data) {
  if ((bare_gtk_controller__observed(gesture) & bare_gtk_gesture_click_event_cancel) == 0) return;

  bare_gtk__emit(bare_gtk_controller__js(gesture), gesture, "cancel", 0, NULL);
}

static js_value_t *
bare_gtk_gesture_click_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  GtkGesture *gesture = gtk_gesture_click_new();

  g_object_set_data(G_OBJECT(gesture), bare_gtk_controller__env, env);

  g_signal_connect(gesture, "pressed", G_CALLBACK(bare_gtk_gesture_click__on_pressed), NULL);
  g_signal_connect(gesture, "released", G_CALLBACK(bare_gtk_gesture_click__on_released), NULL);
  g_signal_connect(gesture, "cancel", G_CALLBACK(bare_gtk_gesture_click__on_cancel), NULL);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(gesture), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_GESTURE_CLICK_H
