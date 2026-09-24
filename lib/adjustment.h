#ifndef BARE_GTK_ADJUSTMENT_H
#define BARE_GTK_ADJUSTMENT_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

// What a scrolled window scrolls is an adjustment, so this is where a position
// is read, written and reported.
enum {
  bare_gtk_adjustment_event_value_changed = 1 << 0,
};

static void
bare_gtk_adjustment__on_value_changed(GtkAdjustment *adjustment, gpointer data) {
  if ((bare_gtk__observed(adjustment) & bare_gtk_adjustment_event_value_changed) == 0) return;

  bare_gtk__emit(bare_gtk__js(adjustment), adjustment, "value-changed", 1, (const double[]) {gtk_adjustment_get_value(adjustment)});
}

static js_value_t *
bare_gtk_adjustment_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  GtkAdjustment *adjustment;
  err = bare_gobject__read_tag(env, argv[0], "adjustment", (gpointer *) &adjustment);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  g_object_set_data(G_OBJECT(adjustment), bare_gtk__events_key, GUINT_TO_POINTER(events));

  // Connected once and left, because the handler costs nothing when the mask
  // says nobody is listening and an adjustment has one signal.
  if (g_object_get_data(G_OBJECT(adjustment), "bare-gtk-connected") == NULL) {
    g_object_set_data(G_OBJECT(adjustment), bare_gtk__env_key, env);
    g_object_set_data(G_OBJECT(adjustment), "bare-gtk-connected", GUINT_TO_POINTER(1));

    g_signal_connect(adjustment, "value-changed", G_CALLBACK(bare_gtk_adjustment__on_value_changed), NULL);
  }

  return NULL;
}

static js_value_t *
bare_gtk_adjustment_value(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkAdjustment *adjustment;
  err = bare_gobject__read_tag(env, argv[0], "adjustment", (gpointer *) &adjustment);
  if (err < 0) return NULL;

  if (argc == 1) {
    js_value_t *result;
    err = js_create_double(env, gtk_adjustment_get_value(adjustment), &result);
    assert(err == 0);

    return result;
  }

  double value;
  err = js_get_value_double(env, argv[1], &value);
  assert(err == 0);

  gtk_adjustment_set_value(adjustment, value);

  return NULL;
}

static js_value_t *
bare_gtk_adjustment_upper(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GtkAdjustment *adjustment;
  err = bare_gobject__read_tag(env, argv[0], "adjustment", (gpointer *) &adjustment);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_create_double(env, gtk_adjustment_get_upper(adjustment), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_ADJUSTMENT_H
