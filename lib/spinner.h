#ifndef BARE_GTK_SPINNER_H
#define BARE_GTK_SPINNER_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_spinner_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  GtkWidget *handle = gtk_spinner_new();

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(handle), &result);
  assert(err == 0);

  return result;
}

// Started and stopped through one property here, where GTK has a method for
// each and a property to ask.
static js_value_t *
bare_gtk_spinner_spinning(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkSpinner *handle;
  err = bare_gobject__read_tag(env, argv[0], "spinner", (gpointer *) &handle);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, gtk_spinner_get_spinning(handle), &result);
    assert(err == 0);
  } else {
    bool spinning;
    err = bare_gtk__read_bool(env, argv[1], "spinning", &spinning);
    if (err < 0) return NULL;

    gtk_spinner_set_spinning(handle, spinning);
  }

  return result;
}

#endif // BARE_GTK_SPINNER_H
