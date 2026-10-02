#ifndef BARE_GTK_ADJUSTMENT_H
#define BARE_GTK_ADJUSTMENT_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

enum {
  bare_gtk_adjustment_event_value_changed = 1 << 0,
};

static void
bare_gtk_adjustment__on_value_changed(GtkAdjustment *adjustment, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, adjustment) & bare_gtk_adjustment_event_value_changed) == 0) return;

  bare_gtk__emit(state, adjustment, "value-changed", 1, (const double[]) {gtk_adjustment_get_value(adjustment)});
}

static js_value_t *
bare_gtk_adjustment_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GtkAdjustment *adjustment;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "adjustment", (gpointer *) &adjustment);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  // Connected once and left connected. The mask decides whether anything is
  // sent.
  if (bare_gtk__observe(state, adjustment, events)) {
    g_signal_connect(adjustment, "value-changed", G_CALLBACK(bare_gtk_adjustment__on_value_changed), state);
  }

  return NULL;
}

static js_value_t *
bare_gtk_adjustment_value(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkAdjustment *adjustment;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "adjustment", (gpointer *) &adjustment);
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

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GtkAdjustment *adjustment;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "adjustment", (gpointer *) &adjustment);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_create_double(env, gtk_adjustment_get_upper(adjustment), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_ADJUSTMENT_H
