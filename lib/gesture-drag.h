#ifndef BARE_GTK_GESTURE_DRAG_H
#define BARE_GTK_GESTURE_DRAG_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"
#include "event-controller.h"

enum {
  bare_gtk_gesture_drag_event_drag_begin = 1 << 0,
  bare_gtk_gesture_drag_event_drag_update = 1 << 1,
  bare_gtk_gesture_drag_event_drag_end = 1 << 2,
  bare_gtk_gesture_drag_event_cancel = 1 << 3,
};

static void
bare_gtk_gesture_drag__on_drag_begin(GtkGestureDrag *gesture, double start_x, double start_y, gpointer data) {
  if ((bare_gtk__observed(gesture) & bare_gtk_gesture_drag_event_drag_begin) == 0) return;

  bare_gtk__emit(bare_gtk__js(gesture), gesture, "drag-begin", 2, (const double[]) {start_x, start_y});
}

static void
bare_gtk_gesture_drag__on_drag_update(GtkGestureDrag *gesture, double offset_x, double offset_y, gpointer data) {
  if ((bare_gtk__observed(gesture) & bare_gtk_gesture_drag_event_drag_update) == 0) return;

  bare_gtk__emit(bare_gtk__js(gesture), gesture, "drag-update", 2, (const double[]) {offset_x, offset_y});
}

static void
bare_gtk_gesture_drag__on_drag_end(GtkGestureDrag *gesture, double offset_x, double offset_y, gpointer data) {
  if ((bare_gtk__observed(gesture) & bare_gtk_gesture_drag_event_drag_end) == 0) return;

  bare_gtk__emit(bare_gtk__js(gesture), gesture, "drag-end", 2, (const double[]) {offset_x, offset_y});
}

static void
bare_gtk_gesture_drag__on_cancel(GtkGesture *gesture, GdkEventSequence *sequence, gpointer data) {
  if ((bare_gtk__observed(gesture) & bare_gtk_gesture_drag_event_cancel) == 0) return;

  bare_gtk__emit(bare_gtk__js(gesture), gesture, "cancel", 0, NULL);
}

static js_value_t *
bare_gtk_gesture_drag_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  GtkGesture *gesture = gtk_gesture_drag_new();

  g_object_set_data(G_OBJECT(gesture), bare_gtk__env_key, env);

  g_signal_connect(gesture, "drag-begin", G_CALLBACK(bare_gtk_gesture_drag__on_drag_begin), NULL);
  g_signal_connect(gesture, "drag-update", G_CALLBACK(bare_gtk_gesture_drag__on_drag_update), NULL);
  g_signal_connect(gesture, "drag-end", G_CALLBACK(bare_gtk_gesture_drag__on_drag_end), NULL);
  g_signal_connect(gesture, "cancel", G_CALLBACK(bare_gtk_gesture_drag__on_cancel), NULL);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(gesture), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_gesture_drag__point(js_env_t *env, double x, double y) {
  int err;

  js_value_t *result;
  err = js_create_array_with_length(env, 2, &result);
  assert(err == 0);

#define V(i, d) \
  { \
    js_value_t *val; \
    err = js_create_double(env, d, &val); \
    assert(err == 0); \
    err = js_set_element(env, result, i, val); \
    assert(err == 0); \
  }

  V(0, x)
  V(1, y)
#undef V

  return result;
}

// Both points answer with nothing while no drag is under way, which is what a
// caller asking outside one has to be able to tell apart from the origin.
static js_value_t *
bare_gtk_gesture_drag_start_point(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GtkGestureDrag *gesture;
  err = bare_gobject__read_tag(env, argv[0], "gesture", (gpointer *) &gesture);
  if (err < 0) return NULL;

  double x, y;

  js_value_t *result;

  if (!gtk_gesture_drag_get_start_point(gesture, &x, &y)) {
    err = js_get_null(env, &result);
    assert(err == 0);

    return result;
  }

  return bare_gtk_gesture_drag__point(env, x, y);
}

static js_value_t *
bare_gtk_gesture_drag_offset(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GtkGestureDrag *gesture;
  err = bare_gobject__read_tag(env, argv[0], "gesture", (gpointer *) &gesture);
  if (err < 0) return NULL;

  double x, y;

  js_value_t *result;

  if (!gtk_gesture_drag_get_offset(gesture, &x, &y)) {
    err = js_get_null(env, &result);
    assert(err == 0);

    return result;
  }

  return bare_gtk_gesture_drag__point(env, x, y);
}

#endif // BARE_GTK_GESTURE_DRAG_H
