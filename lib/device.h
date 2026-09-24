#ifndef BARE_GTK_DEVICE_H
#define BARE_GTK_DEVICE_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_device_source(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GdkDevice *device;
  err = bare_gobject__read_tag(env, argv[0], "device", (gpointer *) &device);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_create_uint32(env, gdk_device_get_source(device), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GTK_DEVICE_H
