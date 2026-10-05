#ifndef BARE_GTK_FRAME_CLOCK_H
#define BARE_GTK_FRAME_CLOCK_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

enum {
  bare_gtk_frame_clock_event_after_paint = 1 << 0,
};

static void
bare_gtk_frame_clock__on_after_paint(GdkFrameClock *clock, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, clock) & bare_gtk_frame_clock_event_after_paint) == 0) return;

  bare_gtk__emit(state, clock, "after-paint", 0, NULL);
}

// Connected once and left connected, so the mask alone decides whether anything
// is sent.
static js_value_t *
bare_gtk_frame_clock_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GdkFrameClock *clock;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "clock", (gpointer *) &clock);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  if (bare_gtk__observe(state, clock, events)) {
    g_signal_connect(clock, "after-paint", G_CALLBACK(bare_gtk_frame_clock__on_after_paint), state);
  }

  return NULL;
}

static js_value_t *
bare_gtk_frame_clock_begin_updating(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkFrameClock *clock;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "clock", (gpointer *) &clock);
  if (err < 0) return NULL;

  gdk_frame_clock_begin_updating(clock);

  return NULL;
}

static js_value_t *
bare_gtk_frame_clock_end_updating(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkFrameClock *clock;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "clock", (gpointer *) &clock);
  if (err < 0) return NULL;

  gdk_frame_clock_end_updating(clock);

  return NULL;
}

static js_value_t *
bare_gtk_frame_clock_frame_counter(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkFrameClock *clock;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "clock", (gpointer *) &clock);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_create_int64(env, gdk_frame_clock_get_frame_counter(clock), &result);
  assert(err == 0);

  return result;
}

// In microseconds, on the monotonic clock.
static js_value_t *
bare_gtk_frame_clock_frame_time(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkFrameClock *clock;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "clock", (gpointer *) &clock);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_create_int64(env, gdk_frame_clock_get_frame_time(clock), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_FRAME_CLOCK_H
