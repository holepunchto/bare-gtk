#ifndef BARE_GTK_WIDGET_H
#define BARE_GTK_WIDGET_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_widget_visible(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, gtk_widget_get_visible(widget), &result);
    assert(err == 0);
  } else {
    bool visible;
    err = bare_gtk__read_bool(env, argv[1], "visible", &visible);
    if (err < 0) return NULL;

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
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

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
    V(1, height)
#undef V
  } else {
    int32_t width;
    err = bare_gtk__read_int32(env, argv[1], "width", &width);
    if (err < 0) return NULL;

    int32_t height;
    err = bare_gtk__read_int32(env, argv[2], "height", &height);
    if (err < 0) return NULL;

    gtk_widget_set_size_request(widget, width, height);
  }

  return result;
}

static js_value_t *
bare_gtk_widget_opacity(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_create_double(env, gtk_widget_get_opacity(widget), &result);
    assert(err == 0);
  } else {
    double opacity;
    err = bare_gtk__read_double(env, argv[1], "opacity", &opacity);
    if (err < 0) return NULL;

    gtk_widget_set_opacity(widget, opacity);
  }

  return result;
}

static js_value_t *
bare_gtk_widget_overflow(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_create_int32(env, gtk_widget_get_overflow(widget), &result);
    assert(err == 0);
  } else {
    int32_t overflow;
    err = bare_gtk__read_int32(env, argv[1], "overflow", &overflow);
    if (err < 0) return NULL;

    gtk_widget_set_overflow(widget, overflow);
  }

  return result;
}

static js_value_t *
bare_gtk_widget_parent(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  return bare_gtk__create_tag(env, gtk_widget_get_parent(widget));
}

static js_value_t *
bare_gtk_widget_layout_manager(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    result = bare_gtk__create_tag(env, gtk_widget_get_layout_manager(widget));
  } else {
    GtkLayoutManager *manager = NULL;
    err = bare_gtk__read_tag_or_null(env, argv[1], "layoutManager", (gpointer *) &manager);
    if (err < 0) return NULL;

    if (manager) g_object_ref(manager);

    gtk_widget_set_layout_manager(widget, manager);
  }

  return result;
}

static js_value_t *
bare_gtk_widget_insert_after(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  GtkWidget *parent;
  err = bare_gobject__read_tag(env, argv[1], "parent", (gpointer *) &parent);
  if (err < 0) return NULL;

  GtkWidget *sibling = NULL;
  err = bare_gtk__read_tag_or_null(env, argv[2], "previousSibling", (gpointer *) &sibling);
  if (err < 0) return NULL;

  gtk_widget_insert_after(widget, parent, sibling);

  return NULL;
}

static js_value_t *
bare_gtk_widget_insert_before(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  GtkWidget *parent;
  err = bare_gobject__read_tag(env, argv[1], "parent", (gpointer *) &parent);
  if (err < 0) return NULL;

  GtkWidget *sibling = NULL;
  err = bare_gtk__read_tag_or_null(env, argv[2], "nextSibling", (gpointer *) &sibling);
  if (err < 0) return NULL;

  gtk_widget_insert_before(widget, parent, sibling);

  return NULL;
}

static js_value_t *
bare_gtk_widget_unparent(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  gtk_widget_unparent(widget);

  return NULL;
}

#define V(name, fn) \
  static js_value_t * \
  name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 1; \
    js_value_t *argv[1]; \
\
    err = js_get_callback_info(env, info, &argc, argv, NULL, NULL); \
    assert(err == 0); \
\
    assert(argc == 1); \
\
    GtkWidget *widget; \
    err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget); \
    if (err < 0) return NULL; \
\
    return bare_gtk__create_tag(env, fn(widget)); \
  }

V(bare_gtk_widget_first_child, gtk_widget_get_first_child)
V(bare_gtk_widget_last_child, gtk_widget_get_last_child)
V(bare_gtk_widget_next_sibling, gtk_widget_get_next_sibling)
V(bare_gtk_widget_prev_sibling, gtk_widget_get_prev_sibling)
#undef V

#define V(name, fn) \
  static js_value_t * \
  name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 1; \
    js_value_t *argv[1]; \
\
    err = js_get_callback_info(env, info, &argc, argv, NULL, NULL); \
    assert(err == 0); \
\
    assert(argc == 1); \
\
    GtkWidget *widget; \
    err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget); \
    if (err < 0) return NULL; \
\
    js_value_t *result; \
    err = js_create_int32(env, fn(widget), &result); \
    assert(err == 0); \
\
    return result; \
  }

V(bare_gtk_widget_width, gtk_widget_get_width)
V(bare_gtk_widget_height, gtk_widget_get_height)
#undef V

static js_value_t *
bare_gtk_widget_compute_bounds(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  GtkWidget *target;
  err = bare_gobject__read_tag(env, argv[1], "target", (gpointer *) &target);
  if (err < 0) return NULL;

  graphene_rect_t bounds;

  js_value_t *result;

  if (!gtk_widget_compute_bounds(widget, target, &bounds)) {
    err = js_get_null(env, &result);
    assert(err == 0);

    return result;
  }

  err = js_create_array_with_length(env, 4, &result);
  assert(err == 0);

#define V(i, d) \
  { \
    js_value_t *val; \
    err = js_create_double(env, d, &val); \
    assert(err == 0); \
    err = js_set_element(env, result, i, val); \
    assert(err == 0); \
  }
  V(0, bounds.origin.x)
  V(1, bounds.origin.y)
  V(2, bounds.size.width)
  V(3, bounds.size.height)
#undef V

  return result;
}

static js_value_t *
bare_gtk_widget_measure(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  int32_t orientation;
  err = bare_gtk__read_int32(env, argv[1], "orientation", &orientation);
  if (err < 0) return NULL;

  int32_t for_size;
  err = bare_gtk__read_int32(env, argv[2], "forSize", &for_size);
  if (err < 0) return NULL;

  int minimum, natural, minimum_baseline, natural_baseline;
  gtk_widget_measure(widget, orientation, for_size, &minimum, &natural, &minimum_baseline, &natural_baseline);

  js_value_t *result;
  err = js_create_array_with_length(env, 4, &result);
  assert(err == 0);

#define V(i, d) \
  { \
    js_value_t *val; \
    err = js_create_int32(env, d, &val); \
    assert(err == 0); \
    err = js_set_element(env, result, i, val); \
    assert(err == 0); \
  }
  V(0, minimum)
  V(1, natural)
  V(2, minimum_baseline)
  V(3, natural_baseline)
#undef V

  return result;
}

#define V(name, fn) \
  static js_value_t * \
  name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 2; \
    js_value_t *argv[2]; \
\
    err = js_get_callback_info(env, info, &argc, argv, NULL, NULL); \
    assert(err == 0); \
\
    assert(argc == 2); \
\
    GtkWidget *widget; \
    err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget); \
    if (err < 0) return NULL; \
\
    char *name_; \
    err = bare_gtk__read_string(env, argv[1], "cssClass", &name_); \
    if (err < 0) return NULL; \
\
    fn(widget, name_); \
\
    g_free(name_); \
\
    return NULL; \
  }

V(bare_gtk_widget_add_css_class, gtk_widget_add_css_class)
V(bare_gtk_widget_remove_css_class, gtk_widget_remove_css_class)
#undef V

static js_value_t *
bare_gtk_widget_has_css_class(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  GtkWidget *widget;
  err = bare_gobject__read_tag(env, argv[0], "widget", (gpointer *) &widget);
  if (err < 0) return NULL;

  char *name;
  err = bare_gtk__read_string(env, argv[1], "cssClass", &name);
  if (err < 0) return NULL;

  js_value_t *result;
  err = js_get_boolean(env, gtk_widget_has_css_class(widget, name), &result);
  assert(err == 0);

  g_free(name);

  return result;
}

#endif // BARE_GTK_WIDGET_H
