#ifndef BARE_GTK_ENTRY_H
#define BARE_GTK_ENTRY_H

#include <assert.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "bridging.h"

enum {
  bare_gtk_entry_event_changed = 1 << 0,
  bare_gtk_entry_event_activate = 1 << 1,
  bare_gtk_entry_event_cursor_position = 1 << 2,
  bare_gtk_entry_event_insert_text = 1 << 3,
  bare_gtk_entry_event_delete_text = 1 << 4,
};

typedef struct {
  GtkEntry handle;

  bare_gtk_state_t *state;

  uint32_t events;

  gulong on_changed;
  gulong on_activate;
  gulong on_cursor_position;
  gulong on_insert_text;
  gulong on_delete_text;
} BareEntry;

typedef struct {
  GtkEntryClass parent;
} BareEntryClass;

G_DEFINE_TYPE(BareEntry, bare_entry, GTK_TYPE_ENTRY)

static void
bare_entry_init(BareEntry *self) {}

static void
bare_entry_class_init(BareEntryClass *class) {}

BareEntry *
bare_entry_new(void) {
  return g_object_new(bare_entry_get_type(), NULL);
}

static void
bare_gtk_entry__on_changed(GtkEditable *editable, gpointer data) {
  BareEntry *self = (BareEntry *) editable;

  bare_gtk__emit(self->state, editable, "changed", 0, NULL);
}

static void
bare_gtk_entry__on_activate(GtkEntry *entry, gpointer data) {
  BareEntry *self = (BareEntry *) entry;

  bare_gtk__emit(self->state, entry, "activate", 0, NULL);
}

static void
bare_gtk_entry__on_cursor_position(GObject *object, GParamSpec *spec, gpointer data) {
  BareEntry *self = (BareEntry *) object;

  bare_gtk__emit(self->state, object, "cursorPosition", 0, NULL);
}

// GTK reports an insertion and a deletion separately, where other toolkits
// report one replacement, so both are sent as a replacement.
static void
bare_gtk_entry__on_insert_text(GtkEditable *editable, const char *text, int length, int *position, gpointer data) {
  BareEntry *self = (BareEntry *) editable;

  bare_gtk__emit_replacement(self->state, editable, "insertText", text, *position, *position);
}

static void
bare_gtk_entry__on_delete_text(GtkEditable *editable, int start, int end, gpointer data) {
  BareEntry *self = (BareEntry *) editable;

  bare_gtk__emit_replacement(self->state, editable, "deleteText", "", start, end);
}

static js_value_t *
bare_gtk_entry_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  BareEntry *entry = bare_entry_new();

  entry->state = state;

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject_tag(state->registry, entry), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_entry_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  BareEntry *entry;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "entry", (gpointer *) &entry);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  uint32_t changed = entry->events ^ events;

  entry->events = events;

#define V(bit, field, signal, callback) \
  if ((changed & (bit)) != 0) { \
    if ((events & (bit)) != 0) { \
      entry->field = g_signal_connect(entry, signal, G_CALLBACK(callback), state); \
    } else { \
      g_signal_handler_disconnect(entry, entry->field); \
    } \
  }

  V(bare_gtk_entry_event_changed, on_changed, "changed", bare_gtk_entry__on_changed)
  V(bare_gtk_entry_event_activate, on_activate, "activate", bare_gtk_entry__on_activate)
  V(bare_gtk_entry_event_cursor_position, on_cursor_position, "notify::cursor-position", bare_gtk_entry__on_cursor_position)
  V(bare_gtk_entry_event_insert_text, on_insert_text, "insert-text", bare_gtk_entry__on_insert_text)
  V(bare_gtk_entry_event_delete_text, on_delete_text, "delete-text", bare_gtk_entry__on_delete_text)
#undef V

  return NULL;
}

static js_value_t *
bare_gtk_entry_placeholder_text(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkEntry *entry;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "entry", (gpointer *) &entry);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    const char *text = gtk_entry_get_placeholder_text(entry);

    if (text) {
      err = js_create_string_utf8(env, (const utf8_t *) text, (size_t) -1, &result);
      assert(err == 0);
    } else {
      err = js_get_null(env, &result);
      assert(err == 0);
    }
  } else {
    char *text;
    err = bare_gtk__read_string_or_null(env, argv[1], "text", &text);
    if (err < 0) return NULL;

    gtk_entry_set_placeholder_text(entry, text);

    g_free(text);
  }

  return result;
}

static js_value_t *
bare_gtk_entry_visibility(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkEntry *entry;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "entry", (gpointer *) &entry);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, gtk_entry_get_visibility(entry), &result);
    assert(err == 0);
  } else {
    bool visibility;
    err = bare_gtk__read_bool(env, argv[1], "visibility", &visibility);
    if (err < 0) return NULL;

    gtk_entry_set_visibility(entry, visibility);
  }

  return result;
}

#define BARE_GTK_ENTRY_ENUM(name, getter, setter, type) \
  static js_value_t * \
  bare_gtk_entry_##name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 2; \
    js_value_t *argv[2]; \
\
    bare_gtk_state_t *state; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state); \
    assert(err == 0); \
\
    assert(argc == 1 || argc == 2); \
\
    GtkEntry *entry; \
    err = bare_gobject_read_tag(env, state->registry, argv[0], "entry", (gpointer *) &entry); \
    if (err < 0) return NULL; \
\
    js_value_t *result = NULL; \
\
    if (argc == 1) { \
      err = js_create_int32(env, (int32_t) getter(entry), &result); \
      assert(err == 0); \
    } else { \
      int32_t value; \
      err = bare_gtk__read_int32(env, argv[1], #name, &value); \
      if (err < 0) return NULL; \
\
      setter(entry, (type) value); \
    } \
\
    return result; \
  }

BARE_GTK_ENTRY_ENUM(input_purpose, gtk_entry_get_input_purpose, gtk_entry_set_input_purpose, GtkInputPurpose)
BARE_GTK_ENTRY_ENUM(input_hints, gtk_entry_get_input_hints, gtk_entry_set_input_hints, GtkInputHints)
#undef BARE_GTK_ENTRY_ENUM

// An entry selects all its text when it gets focus, which would undo a
// selection the caller made. This grab does not.
static js_value_t *
bare_gtk_entry_grab_focus_without_selecting(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GtkEntry *entry;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "entry", (gpointer *) &entry);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_get_boolean(env, gtk_entry_grab_focus_without_selecting(entry), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_entry_alignment(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkEntry *entry;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "entry", (gpointer *) &entry);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_create_double(env, gtk_entry_get_alignment(entry), &result);
    assert(err == 0);
  } else {
    double alignment;
    err = bare_gtk__read_double(env, argv[1], "alignment", &alignment);
    if (err < 0) return NULL;

    gtk_entry_set_alignment(entry, (float) alignment);
  }

  return result;
}

#endif // BARE_GTK_ENTRY_H
