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

static js_value_t *
bare_gtk_widget_size_request(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 3);

  GtkWidget *widget;
  err = js_get_value_external(env, argv[0], (void **) &widget);
  assert(err == 0);

  js_value_t *result = NULL;

  if (argc == 1) {
    int width, height;
    gtk_widget_get_size_request(widget, &width, &height);

    err = js_create_array_with_length(env, 2, &result);
    assert(err == 0);

#define V(i, d) \
  { \
    js_value_t *val; \
    err = js_create_int32(env, d, &val); \
    assert(err == 0); \
    err = js_set_element(env, result, i, val); \
    assert(err == 0); \
  }
    V(0, width)
    V(1, width)
#undef V
  } else {
    int32_t width;
    err = js_get_value_int32(env, argv[1], &width);
    assert(err == 0);

    int32_t height;
    err = js_get_value_int32(env, argv[2], &height);
    assert(err == 0);

    gtk_widget_set_size_request(widget, width, height);
  }

  return result;
}
