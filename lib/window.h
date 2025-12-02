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
  err = js_create_external(env, window, NULL, NULL, &handle);
  assert(err == 0);

  return handle;
}

static js_value_t *
bare_gtk_window_child(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  GtkWindow *window = handle;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_create_external(env, gtk_window_get_child(window), NULL, NULL, &result);
    assert(err == 0);
  } else {
    void *handle;
    err = js_get_value_external(env, argv[1], &handle);
    assert(err == 0);

    gtk_window_set_child(window, handle);
  }

  return result;
}
