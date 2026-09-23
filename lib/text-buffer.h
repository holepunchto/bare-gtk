#ifndef BARE_GTK_TEXT_BUFFER_H
#define BARE_GTK_TEXT_BUFFER_H

#include <assert.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "bridging.h"

enum {
  bare_gtk_text_buffer_event_changed = 1 << 0,
  bare_gtk_text_buffer_event_cursor_position = 1 << 1,
  bare_gtk_text_buffer_event_insert_text = 1 << 2,
  bare_gtk_text_buffer_event_delete_range = 1 << 3,
};

// A text view makes its own buffer, so there is no subclass to keep the mask
// on. It is kept beside the buffer instead.
typedef struct {
  uint32_t events;

  gulong on_changed;
  gulong on_cursor_position;
  gulong on_insert_text;
  gulong on_delete_range;
} bare_gtk_text_buffer_t;

static void
bare_gtk_text_buffer__free(gpointer data, GClosure *closure) {
  g_free(data);
}

static bare_gtk_text_buffer_t *
bare_gtk_text_buffer__signals(GtkTextBuffer *buffer) {
  bare_gtk_text_buffer_t *signals = g_object_get_data(G_OBJECT(buffer), "bare");

  if (signals == NULL) {
    signals = g_new0(bare_gtk_text_buffer_t, 1);

    g_object_set_data_full(G_OBJECT(buffer), "bare", signals, g_free);
  }

  return signals;
}

static void
bare_gtk_text_buffer__on_changed(GtkTextBuffer *buffer, gpointer data) {
  bare_gtk__emit(data, buffer, "changed", 0, NULL);
}

static void
bare_gtk_text_buffer__on_cursor_position(GObject *object, GParamSpec *spec, gpointer data) {
  bare_gtk__emit(data, object, "cursor-position", 0, NULL);
}

static void
bare_gtk_text_buffer__on_insert_text(GtkTextBuffer *buffer, GtkTextIter *position, const char *text, int length, gpointer data) {
  int offset = gtk_text_iter_get_offset(position);

  bare_gtk__emit_replacement(data, buffer, "insert-text", text, offset, offset);
}

static void
bare_gtk_text_buffer__on_delete_range(GtkTextBuffer *buffer, GtkTextIter *start, GtkTextIter *end, gpointer data) {
  bare_gtk__emit_replacement(
    data,
    buffer,
    "delete-range",
    "",
    gtk_text_iter_get_offset(start),
    gtk_text_iter_get_offset(end)
  );
}

static js_value_t *
bare_gtk_text_buffer_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GtkTextBuffer *buffer;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "buffer", (gpointer *) &buffer);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  bare_gtk_text_buffer_t *signals = bare_gtk_text_buffer__signals(buffer);

  uint32_t changed = signals->events ^ events;

  signals->events = events;

#define V(bit, field, signal, callback) \
  if ((changed & (bit)) != 0) { \
    if ((events & (bit)) != 0) { \
      signals->field = g_signal_connect(buffer, signal, G_CALLBACK(callback), state); \
    } else { \
      g_signal_handler_disconnect(buffer, signals->field); \
    } \
  }

  V(bare_gtk_text_buffer_event_changed, on_changed, "changed", bare_gtk_text_buffer__on_changed)
  V(bare_gtk_text_buffer_event_cursor_position, on_cursor_position, "notify::cursor-position", bare_gtk_text_buffer__on_cursor_position)
  V(bare_gtk_text_buffer_event_insert_text, on_insert_text, "insert-text", bare_gtk_text_buffer__on_insert_text)
  V(bare_gtk_text_buffer_event_delete_range, on_delete_range, "delete-range", bare_gtk_text_buffer__on_delete_range)
#undef V

  return NULL;
}

// A `GtkTextIter` has no identity, so it crosses as its offset.
static js_value_t *
bare_gtk_text_buffer_text(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkTextBuffer *buffer;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "buffer", (gpointer *) &buffer);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_bounds(buffer, &start, &end);

    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    err = js_create_string_utf8(env, (const utf8_t *) text, (size_t) -1, &result);
    assert(err == 0);

    g_free(text);
  } else {
    char *text;
    err = bare_gtk__read_string(env, argv[1], "text", &text);
    if (err < 0) return NULL;

    gtk_text_buffer_set_text(buffer, text, -1);

    g_free(text);
  }

  return result;
}

static js_value_t *
bare_gtk_text_buffer_selection_bounds(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GtkTextBuffer *buffer;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "buffer", (gpointer *) &buffer);
  if (err < 0) return NULL;

  GtkTextIter start;
  GtkTextIter end;

  gtk_text_buffer_get_selection_bounds(buffer, &start, &end);

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

  js_value_t *value;

  err = js_create_int32(env, gtk_text_iter_get_offset(&start), &value);
  assert(err == 0);

  err = js_set_named_property(env, result, "start", value);
  assert(err == 0);

  err = js_create_int32(env, gtk_text_iter_get_offset(&end), &value);
  assert(err == 0);

  err = js_set_named_property(env, result, "end", value);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_text_buffer_select_range(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  GtkTextBuffer *buffer;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "buffer", (gpointer *) &buffer);
  if (err < 0) return NULL;

  int32_t start;
  err = bare_gtk__read_int32(env, argv[1], "start", &start);
  if (err < 0) return NULL;

  int32_t end;
  err = bare_gtk__read_int32(env, argv[2], "end", &end);
  if (err < 0) return NULL;

  GtkTextIter from;
  GtkTextIter to;

  gtk_text_buffer_get_iter_at_offset(buffer, &from, start);
  gtk_text_buffer_get_iter_at_offset(buffer, &to, end);

  gtk_text_buffer_select_range(buffer, &to, &from);

  return NULL;
}

#endif // BARE_GTK_TEXT_BUFFER_H
