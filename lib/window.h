#ifndef BARE_GTK_WINDOW_H
#define BARE_GTK_WINDOW_H

#include <assert.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "bridging.h"

extern GtkApplication *bare_gtk_app __attribute__((weak));

enum {
  bare_gtk_window_event_close_request = 1 << 0,
};

typedef struct {
  GtkWindow handle;

  js_env_t *env;

  uint32_t events;
  gulong on_close_request;
} BareWindow;

typedef struct {
  GtkWindowClass parent;
} BareWindowClass;

G_DEFINE_TYPE(BareWindow, bare_window, GTK_TYPE_WINDOW)

static void
bare_window_init(BareWindow *self) {}

static void
bare_window_class_init(BareWindowClass *class) {}

BareWindow *
bare_window_new(void) {
  return g_object_new(bare_window_get_type(), NULL);
}

static gboolean
bare_gtk_window__on_close_request(GtkWindow *window, gpointer data) {
  BareWindow *self = (BareWindow *) window;

  bare_gtk__emit(self->env, window, "close-request", 0, NULL);

  return GDK_EVENT_PROPAGATE;
}

static js_value_t *
bare_gtk_window_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  BareWindow *window = bare_window_new();

  window->env = env;

  if (&bare_gtk_app && bare_gtk_app) gtk_application_add_window(bare_gtk_app, GTK_WINDOW(window));

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(window), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_window_title(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkWindow *window;
  err = bare_gobject__read_tag(env, argv[0], "window", (gpointer *) &window);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    const char *title = gtk_window_get_title(window);

    if (title) {
      err = js_create_string_utf8(env, (const utf8_t *) title, (size_t) -1, &result);
      assert(err == 0);
    } else {
      err = js_get_null(env, &result);
      assert(err == 0);
    }
  } else {
    char *title;
    err = bare_gtk__read_string_or_null(env, argv[1], "title", &title);
    if (err < 0) return NULL;

    gtk_window_set_title(window, title);

    g_free(title);
  }

  return result;
}

static js_value_t *
bare_gtk_window_default_size(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 3);

  GtkWindow *window;
  err = bare_gobject__read_tag(env, argv[0], "window", (gpointer *) &window);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    int width, height;
    gtk_window_get_default_size(window, &width, &height);

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
    V(1, height)
#undef V
  } else {
    int32_t width;
    err = bare_gtk__read_int32(env, argv[1], "width", &width);
    if (err < 0) return NULL;

    int32_t height;
    err = bare_gtk__read_int32(env, argv[2], "height", &height);
    if (err < 0) return NULL;

    gtk_window_set_default_size(window, width, height);
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

  GtkWindow *window;
  err = bare_gobject__read_tag(env, argv[0], "window", (gpointer *) &window);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    result = bare_gtk__create_tag(env, gtk_window_get_child(window));
  } else {
    bool is;
    err = js_is_null(env, argv[1], &is);
    assert(err == 0);

    GtkWidget *child = NULL;

    if (!is) {
      err = bare_gobject__read_tag(env, argv[1], "child", (gpointer *) &child);
      if (err < 0) return NULL;
    }

    gtk_window_set_child(window, child);
  }

  return result;
}

static js_value_t *
bare_gtk_window_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  BareWindow *window;
  err = bare_gobject__read_type(env, argv[0], "window", bare_window_get_type(), (gpointer *) &window);
  if (err < 0) return NULL;

  if (argc == 1) {
    js_value_t *result;
    err = js_create_uint32(env, window->events, &result);
    assert(err == 0);

    return result;
  }

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  uint32_t changed = window->events ^ events;

  if (changed & bare_gtk_window_event_close_request) {
    if (events & bare_gtk_window_event_close_request) {
      window->on_close_request = g_signal_connect(window, "close-request", G_CALLBACK(bare_gtk_window__on_close_request), NULL);
    } else {
      g_signal_handler_disconnect(window, window->on_close_request);

      window->on_close_request = 0;
    }
  }

  window->events = events;

  return NULL;
}

#endif // BARE_GTK_WINDOW_H
