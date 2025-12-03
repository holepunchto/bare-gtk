#include <assert.h>
#include <bare.h>
#include <js.h>

#include <gtk/gtk.h>

extern GtkApplication *bare_gtk_app;

typedef struct {
  GtkWindow handle;

  js_env_t *env;
  js_ref_t *ctx;
} BareWindow;

typedef struct {
  GtkWindowClass parent;
} BareWindowClass;

G_DEFINE_TYPE(BareWindow, bare_window, GTK_TYPE_WINDOW)

static void
bare_window_init(BareWindow *self) {}

BareWindow *
bare_window_new(void) {
  return g_object_new(bare_window_get_type(), NULL);
}

static void
bare_window_finalize(GObject *object) {
  int err;

  BareWindow *self = (BareWindow *) object;

  js_env_t *env = self->env;

  err = js_delete_reference(env, self->ctx);
  assert(err == 0);

  G_OBJECT_CLASS(bare_window_parent_class)->finalize(object);
}

static void
bare_window_class_init(BareWindowClass *class) {
  GObjectClass *object_class = G_OBJECT_CLASS(class);

  object_class->finalize = bare_window_finalize;
}

static void
bare_gtk_window__on_release(js_env_t *env, void *data, void *finalize_hint) {
  g_object_unref(data);
}

static js_value_t *
bare_gtk_window_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  BareWindow *window = bare_window_new();

  gtk_application_add_window(bare_gtk_app, GTK_WINDOW(window));

  window->env = env;

  err = js_create_reference(env, argv[0], 1, &window->ctx);
  assert(err == 0);

  js_value_t *handle;
  err = js_create_external(env, window, bare_gtk_window__on_release, NULL, &handle);
  assert(err == 0);

  return handle;
}

static js_value_t *
bare_gtk_window_default_size(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 3);

  BareWindow *window;
  err = js_get_value_external(env, argv[0], (void **) &window);
  assert(err == 0);

  js_value_t *result = NULL;

  if (argc == 1) {
    int width, height;
    gtk_window_get_default_size(GTK_WINDOW(window), &width, &height);

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

    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
  }

  return result;
}

static js_value_t *
bare_gtk_window_child(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  BareWindow *window;
  err = js_get_value_external(env, argv[0], (void **) &window);
  assert(err == 0);

  js_value_t *result = NULL;

  if (argc == 1) {
    GtkWidget *handle = gtk_window_get_child(GTK_WINDOW(window));

    if (handle) {
      err = js_create_external(env, handle, NULL, NULL, &result);
      assert(err == 0);
    } else {
      err = js_get_null(env, &result);
      assert(err == 0);
    }
  } else {
    void *handle;
    err = js_get_value_external(env, argv[1], &handle);
    assert(err == 0);

    gtk_window_set_child(GTK_WINDOW(window), handle);
  }

  return result;
}
