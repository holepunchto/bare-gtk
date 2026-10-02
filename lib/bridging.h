#ifndef BARE_GTK_BRIDGING_H
#define BARE_GTK_BRIDGING_H

#include <assert.h>
#include <glib.h>
#include <js.h>
#include <stdbool.h>
#include <stdint.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "registry.h"

// What the addon keeps between calls. It belongs to one instantiation, because
// an addon can be loaded more than once in a process. Bindings reach it through
// their data pointer, and signal handlers through the object they are connected
// to.
typedef struct {
  bare_gobject_registry_t *registry;
  js_env_t *env;

  GHashTable *events;

  // A label that is never shown, used to measure text in the font a real label
  // draws with. A standalone Pango context would pick a different font. A theme
  // may pad a label, so its inset is measured too.
  GtkWidget *text_probe;
  int text_inset_width;
  int text_inset_height;
} bare_gtk_state_t;

static void
bare_gtk__on_observed_gone(gpointer data, GObject *object);

static void
bare_gtk__on_state_release(js_env_t *env, void *data, void *finalize_hint) {
  bare_gtk_state_t *state = data;

  GHashTableIter iter;
  gpointer object;

  g_hash_table_iter_init(&iter, state->events);

  while (g_hash_table_iter_next(&iter, &object, NULL)) {
    g_object_weak_unref(G_OBJECT(object), bare_gtk__on_observed_gone, state);
  }

  g_hash_table_destroy(state->events);

  if (state->text_probe) g_object_unref(state->text_probe);

  bare_gobject_registry_release(state->registry);

  g_free(state);
}

static bare_gtk_state_t *
bare_gtk_state_create(js_env_t *env, js_value_t *exports) {
  int err;

  bare_gtk_state_t *state = g_new0(bare_gtk_state_t, 1);

  state->registry = bare_gobject_registry_create(env, exports);
  state->env = env;

  state->events = g_hash_table_new(NULL, NULL);

  err = js_add_finalizer(env, exports, state, bare_gtk__on_state_release, NULL, NULL);
  assert(err == 0);

  return state;
}

static int
bare_gtk__read_bool(js_env_t *env, js_value_t *value, const char *name, bool *result) {
  int err;

  bool is;
  err = js_is_boolean(env, value, &is);
  assert(err == 0);

  if (!is) {
    err = js_throw_type_errorf(env, NULL, "Expected '%s' to be a boolean", name);
    assert(err == 0);

    return -1;
  }

  err = js_get_value_bool(env, value, result);
  assert(err == 0);

  return 0;
}

static int
bare_gtk__read_number(js_env_t *env, js_value_t *value, const char *name) {
  int err;

  bool is;
  err = js_is_number(env, value, &is);
  assert(err == 0);

  if (!is) {
    err = js_throw_type_errorf(env, NULL, "Expected '%s' to be a number", name);
    assert(err == 0);

    return -1;
  }

  return 0;
}

static int
bare_gtk__read_int32(js_env_t *env, js_value_t *value, const char *name, int32_t *result) {
  int err;

  err = bare_gtk__read_number(env, value, name);
  if (err < 0) return err;

  err = js_get_value_int32(env, value, result);
  assert(err == 0);

  return 0;
}

static int
bare_gtk__read_uint32(js_env_t *env, js_value_t *value, const char *name, uint32_t *result) {
  int err;

  err = bare_gtk__read_number(env, value, name);
  if (err < 0) return err;

  err = js_get_value_uint32(env, value, result);
  assert(err == 0);

  return 0;
}

static int
bare_gtk__read_double(js_env_t *env, js_value_t *value, const char *name, double *result) {
  int err;

  err = bare_gtk__read_number(env, value, name);
  if (err < 0) return err;

  err = js_get_value_double(env, value, result);
  assert(err == 0);

  return 0;
}

static int
bare_gtk__read_string(js_env_t *env, js_value_t *value, const char *name, char **result) {
  int err;

  bool is;
  err = js_is_string(env, value, &is);
  assert(err == 0);

  if (!is) {
    err = js_throw_type_errorf(env, NULL, "Expected '%s' to be a string", name);
    assert(err == 0);

    return -1;
  }

  size_t len;
  err = js_get_value_string_utf8(env, value, NULL, 0, &len);
  assert(err == 0);

  char *str = g_malloc(len + 1);

  err = js_get_value_string_utf8(env, value, (utf8_t *) str, len + 1, NULL);
  assert(err == 0);

  *result = str;

  return 0;
}

static int
bare_gtk__read_string_or_null(js_env_t *env, js_value_t *value, const char *name, char **result) {
  int err;

  bool is;
  err = js_is_null(env, value, &is);
  assert(err == 0);

  if (is) {
    *result = NULL;

    return 0;
  }

  return bare_gtk__read_string(env, value, name, result);
}

// A GTK call that fails sets a `GError`. Here that becomes a thrown error.
static int
bare_gtk__throw(js_env_t *env, GError *error) {
  int err = js_throw_error(env, NULL, error->message);
  assert(err == 0);

  g_error_free(error);

  return -1;
}

static int
bare_gtk__read_tag_or_null(js_env_t *env, bare_gobject_registry_t *registry, js_value_t *value, const char *name, gpointer *result) {
  int err;

  bool is;
  err = js_is_null(env, value, &is);
  assert(err == 0);

  if (is) {
    *result = NULL;

    return 0;
  }

  return bare_gobject_read_tag(env, registry, value, name, result);
}

static js_value_t *
bare_gtk__create_tag(js_env_t *env, bare_gtk_state_t *state, gpointer object) {
  int err;

  js_value_t *result;

  if (object) {
    err = js_create_uint32(env, bare_gobject_tag(state->registry, object), &result);
  } else {
    err = js_get_null(env, &result);
  }

  assert(err == 0);

  return result;
}

// Each instantiation keeps its own event mask for an object, because objects
// such as `GtkSettings` are shared by the whole process. The weak reference
// removes the entry when the object goes away.
static void
bare_gtk__on_observed_gone(gpointer data, GObject *object) {
  bare_gtk_state_t *state = data;

  g_hash_table_remove(state->events, object);
}

static uint32_t
bare_gtk__observed(bare_gtk_state_t *state, gpointer object) {
  return GPOINTER_TO_UINT(g_hash_table_lookup(state->events, object));
}

// Returns true the first time this state sees the object, which is when it
// still has to connect its signals.
static bool
bare_gtk__observe(bare_gtk_state_t *state, gpointer object, uint32_t events) {
  if (!g_hash_table_insert(state->events, object, GUINT_TO_POINTER(events))) return false;

  g_object_weak_ref(G_OBJECT(object), bare_gtk__on_observed_gone, state);

  return true;
}

// Emit an edit before it happens, as the range it replaces and the new text.
static void
bare_gtk__emit_replacement(bare_gtk_state_t *state, gpointer object, const char *event, const char *text, int start, int end) {
  int err;

  js_env_t *env = state->env;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *wrapper = bare_gobject_lookup(env, state->registry, object);

  if (wrapper) {
    js_value_t *emit;
    err = js_get_named_property(env, wrapper, "emit", &emit);
    assert(err == 0);

    js_value_t *argv[4];

    err = js_create_string_utf8(env, (const utf8_t *) event, (size_t) -1, &argv[0]);
    assert(err == 0);

    err = js_create_string_utf8(env, (const utf8_t *) (text == NULL ? "" : text), (size_t) -1, &argv[1]);
    assert(err == 0);

    err = js_create_int32(env, start, &argv[2]);
    assert(err == 0);

    err = js_create_int32(env, end, &argv[3]);
    assert(err == 0);

    err = js_call_function(env, wrapper, emit, 4, argv, NULL);
    assert(err == 0 || err == js_pending_exception);
  }

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

static void
bare_gtk__emit(bare_gtk_state_t *state, gpointer object, const char *event, size_t argc, const double args[]) {
  int err;

  js_env_t *env = state->env;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *wrapper = bare_gobject_lookup(env, state->registry, object);

  if (wrapper) {
    js_value_t *emit;
    err = js_get_named_property(env, wrapper, "emit", &emit);
    assert(err == 0);

    js_value_t *argv[8];

    assert(argc + 1 <= 8);

    err = js_create_string_utf8(env, (const utf8_t *) event, (size_t) -1, &argv[0]);
    assert(err == 0);

    for (size_t i = 0; i < argc; i++) {
      err = js_create_double(env, args[i], &argv[i + 1]);
      assert(err == 0);
    }

    err = js_call_function(env, wrapper, emit, argc + 1, argv, NULL);
    assert(err == 0 || err == js_pending_exception);
  }

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

#endif // BARE_GTK_BRIDGING_H
