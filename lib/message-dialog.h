#ifndef BARE_GTK_MESSAGE_DIALOG_H
#define BARE_GTK_MESSAGE_DIALOG_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

enum {
  bare_gtk_dialog_event_response = 1 << 0,
};

static void
bare_gtk_dialog__on_response(GtkDialog *dialog, gint response, gpointer data) {
  bare_gtk_state_t *state = data;

  if ((bare_gtk__observed(state, dialog) & bare_gtk_dialog_event_response) == 0) return;

  bare_gtk__emit(state, dialog, "response", 1, (double[]){(double) response});
}

// No buttons yet, because adding them one at a time is the only way to choose
// their labels.
static js_value_t *
bare_gtk_message_dialog_new(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  GtkWindow *parent = NULL;

  js_value_type_t type;
  err = js_typeof(env, argv[0], &type);
  assert(err == 0);

  if (type != js_null && type != js_undefined) {
    err = bare_gobject_read_tag(env, state->registry, argv[0], "parent", (gpointer *) &parent);
    if (err < 0) return NULL;
  }

  int32_t message_type;
  err = bare_gtk__read_int32(env, argv[1], "message_type", &message_type);
  if (err < 0) return NULL;

  char *text;
  err = bare_gtk__read_string(env, argv[2], "text", &text);
  if (err < 0) return NULL;

  GtkWidget *dialog = gtk_message_dialog_new(
    parent,
    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
    (GtkMessageType) message_type,
    GTK_BUTTONS_NONE,
    "%s",
    text
  );

  free(text);

  g_signal_connect(dialog, "response", G_CALLBACK(bare_gtk_dialog__on_response), state);

  return bare_gtk__create_tag(env, state, dialog);
}

static js_value_t *
bare_gtk_message_dialog_secondary_text(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GtkMessageDialog *dialog;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "dialog", (gpointer *) &dialog);
  if (err < 0) return NULL;

  char *text;
  err = bare_gtk__read_string(env, argv[1], "text", &text);
  if (err < 0) return NULL;

  gtk_message_dialog_format_secondary_text(dialog, "%s", text);

  free(text);

  return NULL;
}

static js_value_t *
bare_gtk_dialog_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GtkDialog *dialog;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "dialog", (gpointer *) &dialog);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  bare_gtk__observe(state, dialog, events);

  return NULL;
}

static js_value_t *
bare_gtk_dialog_add_button(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  GtkDialog *dialog;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "dialog", (gpointer *) &dialog);
  if (err < 0) return NULL;

  char *label;
  err = bare_gtk__read_string(env, argv[1], "label", &label);
  if (err < 0) return NULL;

  int32_t response;
  err = bare_gtk__read_int32(env, argv[2], "response", &response);
  if (err < 0) {
    free(label);

    return NULL;
  }

  GtkWidget *button = gtk_dialog_add_button(dialog, label, response);

  free(label);

  return bare_gtk__create_tag(env, state, button);
}

static js_value_t *
bare_gtk_dialog_response(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  GtkDialog *dialog;
  err = bare_gobject_read_tag(env, state->registry, argv[0], "dialog", (gpointer *) &dialog);
  if (err < 0) return NULL;

  int32_t response;
  err = bare_gtk__read_int32(env, argv[1], "response", &response);
  if (err < 0) return NULL;

  gtk_dialog_response(dialog, response);

  return NULL;
}

#endif // BARE_GTK_MESSAGE_DIALOG_H
