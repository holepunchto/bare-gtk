#include <assert.h>
#include <bare.h>
#include <js.h>

#include <gtk/gtk.h>

extern GtkApplication *bare_gtk_app;

typedef struct {
  GtkWindow handle;
} BareWindow;

typedef struct {
  GtkWindowClass parent;
} BareWindowClass;

G_DEFINE_TYPE(BareWindow, bare_window, GTK_TYPE_WINDOW)

static void
bare_window_class_init(BareWindowClass *class) {}

static void
bare_window_init(BareWindow *self) {}

BareWindow *
bare_window_new(void) {
  return g_object_new(bare_window_get_type(), NULL);
}

static void
bare_window_finalize(GObject *object) {
  BareWindow *self = (BareWindow *) object;

  G_OBJECT_CLASS(bare_window_parent_class)->finalize(object);
}

static void
bare_gtk_window__on_release(js_env_t *env, void *data, void *finalize_hint) {
  gtk_window_destroy(GTK_WINDOW(data));
}

static js_value_t *
bare_gtk_window_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 0;
  js_value_t *argv[0];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 0);

  BareWindow *window = bare_window_new();

  gtk_application_add_window(bare_gtk_app, GTK_WINDOW(window));

  js_value_t *handle;
  err = js_create_external(env, window, bare_gtk_window__on_release, NULL, &handle);
  assert(err == 0);

  return handle;
}
