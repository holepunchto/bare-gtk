#ifndef BARE_GTK_TEXT_VIEW_H
#define BARE_GTK_TEXT_VIEW_H

#include <assert.h>
#include <js.h>
#include <utf.h>

#include <gtk/gtk.h>

#include "bridging.h"

enum {
  bare_gtk_text_view_event_has_focus = 1 << 0,
  bare_gtk_text_view_event_is_focus = 1 << 1,
};

typedef struct {
  GtkTextView handle;

  js_env_t *env;

  uint32_t events;

  gulong on_has_focus;
  gulong on_is_focus;
} BareTextView;

typedef struct {
  GtkTextViewClass parent;
} BareTextViewClass;

G_DEFINE_TYPE(BareTextView, bare_text_view, GTK_TYPE_TEXT_VIEW)

static void
bare_text_view_init(BareTextView *self) {}

static void
bare_text_view_class_init(BareTextViewClass *class) {}

BareTextView *
bare_text_view_new(void) {
  return g_object_new(bare_text_view_get_type(), NULL);
}

static void
bare_gtk_text_view__on_has_focus(GObject *object, GParamSpec *spec, gpointer data) {
  BareTextView *self = (BareTextView *) object;

  bare_gtk__emit(self->env, object, "has-focus", 1, (const double[]) {gtk_widget_has_focus(GTK_WIDGET(object)) ? 1 : 0});
}

// Whether this is the focus of the window it is in, which is a different thing
// from holding the keyboard.
static void
bare_gtk_text_view__on_is_focus(GObject *object, GParamSpec *spec, gpointer data) {
  BareTextView *self = (BareTextView *) object;

  bare_gtk__emit(self->env, object, "is-focus", 1, (const double[]) {gtk_widget_is_focus(GTK_WIDGET(object)) ? 1 : 0});
}

static js_value_t *
bare_gtk_text_view_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  BareTextView *view = bare_text_view_new();

  view->env = env;

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(view), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_text_view_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  BareTextView *view;
  err = bare_gobject__read_tag(env, argv[0], "textView", (gpointer *) &view);
  if (err < 0) return NULL;

  uint32_t events;
  err = bare_gtk__read_uint32(env, argv[1], "events", &events);
  if (err < 0) return NULL;

  uint32_t changed = view->events ^ events;

  view->events = events;

#define V(bit, field, signal, callback) \
  if ((changed & (bit)) != 0) { \
    if ((events & (bit)) != 0) { \
      view->field = g_signal_connect(view, signal, G_CALLBACK(callback), NULL); \
    } else { \
      g_signal_handler_disconnect(view, view->field); \
    } \
  }

  V(bare_gtk_text_view_event_has_focus, on_has_focus, "notify::has-focus", bare_gtk_text_view__on_has_focus)
  V(bare_gtk_text_view_event_is_focus, on_is_focus, "notify::is-focus", bare_gtk_text_view__on_is_focus)
#undef V

  return NULL;
}

static js_value_t *
bare_gtk_text_view_buffer(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GtkTextView *view;
  err = bare_gobject__read_tag(env, argv[0], "textView", (gpointer *) &view);
  if (err < 0) return NULL;

  return bare_gtk__create_tag(env, gtk_text_view_get_buffer(view));
}

static js_value_t *
bare_gtk_text_view_editable(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkTextView *view;
  err = bare_gobject__read_tag(env, argv[0], "textView", (gpointer *) &view);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, gtk_text_view_get_editable(view), &result);
    assert(err == 0);
  } else {
    bool editable;
    err = bare_gtk__read_bool(env, argv[1], "editable", &editable);
    if (err < 0) return NULL;

    gtk_text_view_set_editable(view, editable);
  }

  return result;
}

#define BARE_GTK_TEXT_VIEW_ENUM(name, getter, setter, type) \
  static js_value_t * \
  bare_gtk_text_view_##name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 2; \
    js_value_t *argv[2]; \
\
    err = js_get_callback_info(env, info, &argc, argv, NULL, NULL); \
    assert(err == 0); \
\
    assert(argc == 1 || argc == 2); \
\
    GtkTextView *view; \
    err = bare_gobject__read_tag(env, argv[0], "textView", (gpointer *) &view); \
    if (err < 0) return NULL; \
\
    js_value_t *result = NULL; \
\
    if (argc == 1) { \
      err = js_create_int32(env, (int32_t) getter(view), &result); \
      assert(err == 0); \
    } else { \
      int32_t value; \
      err = bare_gtk__read_int32(env, argv[1], #name, &value); \
      if (err < 0) return NULL; \
\
      setter(view, (type) value); \
    } \
\
    return result; \
  }

BARE_GTK_TEXT_VIEW_ENUM(wrap_mode, gtk_text_view_get_wrap_mode, gtk_text_view_set_wrap_mode, GtkWrapMode)
BARE_GTK_TEXT_VIEW_ENUM(input_purpose, gtk_text_view_get_input_purpose, gtk_text_view_set_input_purpose, GtkInputPurpose)
BARE_GTK_TEXT_VIEW_ENUM(input_hints, gtk_text_view_get_input_hints, gtk_text_view_set_input_hints, GtkInputHints)
#undef BARE_GTK_TEXT_VIEW_ENUM


static js_value_t *
bare_gtk_text_view_justification(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkTextView *text_view;
  err = bare_gobject__read_tag(env, argv[0], "textView", (gpointer *) &text_view);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_create_int32(env, gtk_text_view_get_justification(text_view), &result);
    assert(err == 0);
  } else {
    int32_t justification;
    err = bare_gtk__read_int32(env, argv[1], "justification", &justification);
    if (err < 0) return NULL;

    gtk_text_view_set_justification(text_view, justification);
  }

  return result;
}

#endif // BARE_GTK_TEXT_VIEW_H
