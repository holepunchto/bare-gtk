#ifndef BARE_GTK_CLIPBOARD_H
#define BARE_GTK_CLIPBOARD_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

// GDK reads a clipboard through a callback, so the caller passes one in.
typedef struct {
  bare_gtk_state_t *state;
  js_ref_t *callback;
} bare_gtk_clipboard_read_t;

static void
bare_gtk_clipboard__on_read_text(GObject *source, GAsyncResult *result, gpointer data) {
  int err;

  bare_gtk_clipboard_read_t *read = data;

  js_env_t *env = read->state->env;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *callback;
  err = js_get_reference_value(env, read->callback, &callback);
  assert(err == 0);

  GError *error = NULL;

  char *text = gdk_clipboard_read_text_finish(GDK_CLIPBOARD(source), result, &error);

  js_value_t *argv[2];

  if (error == NULL) {
    err = js_get_null(env, &argv[0]);
    assert(err == 0);

    if (text == NULL) {
      err = js_get_null(env, &argv[1]);
    } else {
      err = js_create_string_utf8(env, (const utf8_t *) text, (size_t) -1, &argv[1]);
    }

    assert(err == 0);
  } else {
    err = js_create_string_utf8(env, (const utf8_t *) error->message, (size_t) -1, &argv[0]);
    assert(err == 0);

    err = js_get_null(env, &argv[1]);
    assert(err == 0);

    g_error_free(error);
  }

  g_free(text);

  js_value_t *receiver;
  err = js_get_null(env, &receiver);
  assert(err == 0);

  err = js_call_function(env, receiver, callback, 2, argv, NULL);
  (void) err;

  err = js_delete_reference(env, read->callback);
  assert(err == 0);

  err = js_close_handle_scope(env, scope);
  assert(err == 0);

  free(read);
}

static js_value_t *
bare_gtk_clipboard_read_text_async(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GdkClipboard *clipboard;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "clipboard", (gpointer *) &clipboard);
  if (err < 0) return NULL;

  bare_gtk_clipboard_read_t *read = malloc(sizeof(bare_gtk_clipboard_read_t));

  read->state = state;

  err = js_create_reference(env, argv[1], 1, &read->callback);
  assert(err == 0);

  gdk_clipboard_read_text_async(clipboard, NULL, bare_gtk_clipboard__on_read_text, read);

  return NULL;
}

static js_value_t *
bare_gtk_clipboard_set_text(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GdkClipboard *clipboard;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "clipboard", (gpointer *) &clipboard);
  if (err < 0) return NULL;

  char *text;
  err = bare_gtk__read_string(env, argv[1], "text", &text);
  if (err < 0) return NULL;

  gdk_clipboard_set_text(clipboard, text);

  free(text);

  return NULL;
}

// A `GdkContentFormats` is not a `GObject`, so it crosses as the list of mime
// types it holds.
static js_value_t *
bare_gtk_clipboard_formats(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  GdkClipboard *clipboard;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "clipboard", (gpointer *) &clipboard);
  if (err < 0) return NULL;

  GdkContentFormats *formats = gdk_clipboard_get_formats(clipboard);

  gsize len = 0;

  const char *const *mime_types = gdk_content_formats_get_mime_types(formats, &len);

  js_value_t *result;
  err = js_create_array_with_length(env, len, &result);
  assert(err == 0);

  for (gsize i = 0; i < len; i++) {
    js_value_t *value;
    err = js_create_string_utf8(env, (const utf8_t *) mime_types[i], (size_t) -1, &value);
    assert(err == 0);

    err = js_set_element(env, result, i, value);
    assert(err == 0);
  }

  return result;
}

#endif // BARE_GTK_CLIPBOARD_H
