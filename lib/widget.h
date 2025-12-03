#include <assert.h>
#include <bare.h>
#include <js.h>

#include <gtk/gtk.h>

static js_value_t *
bare_gtk_widget_visible(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  GtkWidget *widget = handle;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, gtk_widget_get_visible(widget), &result);
    assert(err == 0);
  } else {
    bool visible;
    err = js_get_value_bool(env, argv[1], &visible);
    assert(err == 0);

    gtk_widget_set_visible(widget, visible);
  }

  return result;
}
