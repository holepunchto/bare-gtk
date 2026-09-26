#ifndef BARE_GTK_FRAME_LAYOUT_H
#define BARE_GTK_FRAME_LAYOUT_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

// Gives each child an outright frame. No stock manager does: they size a child
// from its own preferred size, and a size request only raises a minimum.

enum {
  bare_gtk_frame_layout_event_resize = 1 << 0,
};

#define BARE_TYPE_FRAME_LAYOUT_CHILD (bare_frame_layout_child_get_type())

G_DECLARE_FINAL_TYPE(BareFrameLayoutChild, bare_frame_layout_child, BARE, FRAME_LAYOUT_CHILD, GtkLayoutChild)

struct _BareFrameLayoutChild {
  GtkLayoutChild handle;

  graphene_rect_t frame;
  graphene_matrix_t transform;
  bool transformed;
};

G_DEFINE_TYPE(BareFrameLayoutChild, bare_frame_layout_child, GTK_TYPE_LAYOUT_CHILD)

static void
bare_frame_layout_child_init(BareFrameLayoutChild *self) {
  self->frame = GRAPHENE_RECT_INIT(0, 0, 0, 0);
  self->transformed = false;
}

static void
bare_frame_layout_child_class_init(BareFrameLayoutChildClass *class) {}

#define BARE_TYPE_FRAME_LAYOUT (bare_frame_layout_get_type())

G_DECLARE_FINAL_TYPE(BareFrameLayout, bare_frame_layout, BARE, FRAME_LAYOUT, GtkLayoutManager)

struct _BareFrameLayout {
  GtkLayoutManager handle;

  js_env_t *env;

  uint32_t events;
  int width, height;
};

G_DEFINE_TYPE(BareFrameLayout, bare_frame_layout, GTK_TYPE_LAYOUT_MANAGER)

static void
bare_frame_layout_measure(GtkLayoutManager *manager, GtkWidget *widget, GtkOrientation orientation, int for_size, int *minimum, int *natural, int *minimum_baseline, int *natural_baseline) {
  int content = 0;

  for (GtkWidget *child = gtk_widget_get_first_child(widget); child; child = gtk_widget_get_next_sibling(child)) {
    BareFrameLayoutChild *layout_child = BARE_FRAME_LAYOUT_CHILD(gtk_layout_manager_get_layout_child(manager, child));

    graphene_rect_t *frame = &layout_child->frame;

    int edge = orientation == GTK_ORIENTATION_HORIZONTAL
      ? (int) (frame->origin.x + frame->size.width)
      : (int) (frame->origin.y + frame->size.height);

    if (edge > content) content = edge;
  }

  // A scrolled window asks its child how big it wants to be, and a layout that
  // knows where it put everything knows what that comes to. The minimum stays
  // zero, so a viewport is free to be smaller than what it holds.
  *minimum = 0;
  *natural = content;
  *minimum_baseline = -1;
  *natural_baseline = -1;
}

static void
bare_frame_layout_allocate(GtkLayoutManager *manager, GtkWidget *widget, int width, int height, int baseline) {
  BareFrameLayout *self = BARE_FRAME_LAYOUT(manager);

  // Before the children are placed, so a listener setting frames is honoured in
  // this pass. Such a listener must not restructure the tree.
  if (self->width != width || self->height != height) {
    self->width = width;
    self->height = height;

    if (self->env && self->events & bare_gtk_frame_layout_event_resize) {
      bare_gtk__emit(self->env, self, "resize", 2, (const double[]) {width, height});
    }
  }

  for (GtkWidget *child = gtk_widget_get_first_child(widget); child; child = gtk_widget_get_next_sibling(child)) {
    BareFrameLayoutChild *layout_child = BARE_FRAME_LAYOUT_CHILD(gtk_layout_manager_get_layout_child(manager, child));

    graphene_rect_t *frame = &layout_child->frame;

    GskTransform *transform = gsk_transform_translate(NULL, &GRAPHENE_POINT_INIT(frame->origin.x, frame->origin.y));

    // A widget cannot transform itself: what it is drawn through is the
    // transform its parent allocates it with, so a child's own is composed
    // onto the translation that places it.
    if (layout_child->transformed) {
      transform = gsk_transform_matrix(transform, &layout_child->transform);
    }

    gtk_widget_allocate(child, (int) frame->size.width, (int) frame->size.height, -1, transform);
  }
}

static GtkLayoutChild *
bare_frame_layout_create_layout_child(GtkLayoutManager *manager, GtkWidget *widget, GtkWidget *for_child) {
  return g_object_new(BARE_TYPE_FRAME_LAYOUT_CHILD, "layout-manager", manager, "child-widget", for_child, NULL);
}

static void
bare_frame_layout_init(BareFrameLayout *self) {}

static void
bare_frame_layout_class_init(BareFrameLayoutClass *class) {
  GtkLayoutManagerClass *layout_class = GTK_LAYOUT_MANAGER_CLASS(class);

  layout_class->measure = bare_frame_layout_measure;
  layout_class->allocate = bare_frame_layout_allocate;
  layout_class->create_layout_child = bare_frame_layout_create_layout_child;
  layout_class->layout_child_type = BARE_TYPE_FRAME_LAYOUT_CHILD;
}

static js_value_t *
bare_gtk_frame_layout_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  BareFrameLayout *layout = g_object_new(BARE_TYPE_FRAME_LAYOUT, NULL);

  layout->env = env;

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(layout), &result);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_gtk_frame_layout_event_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  BareFrameLayout *layout;
  err = bare_gobject__read_type(env, argv[0], "frameLayout", BARE_TYPE_FRAME_LAYOUT, (gpointer *) &layout);
  if (err < 0) return NULL;

  if (argc == 1) {
    js_value_t *result;
    err = js_create_uint32(env, layout->events, &result);
    assert(err == 0);

    return result;
  }

  err = bare_gtk__read_uint32(env, argv[1], "events", &layout->events);
  if (err < 0) return NULL;

  return NULL;
}

static js_value_t *
bare_gtk_frame_layout_child_frame(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 5;
  js_value_t *argv[5];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 5);

  BareFrameLayoutChild *layout_child;
  err = bare_gobject__read_type(env, argv[0], "layoutChild", BARE_TYPE_FRAME_LAYOUT_CHILD, (gpointer *) &layout_child);
  if (err < 0) return NULL;

  js_value_t *result = NULL;

  if (argc == 1) {
    graphene_rect_t *frame = &layout_child->frame;

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
    V(0, frame->origin.x)
    V(1, frame->origin.y)
    V(2, frame->size.width)
    V(3, frame->size.height)
#undef V
  } else {
    double frame[4];

#define V(i, name) \
  { \
    err = bare_gtk__read_double(env, argv[i + 1], name, &frame[i]); \
    if (err < 0) return NULL; \
  }
    V(0, "x")
    V(1, "y")
    V(2, "width")
    V(3, "height")
#undef V

    layout_child->frame = GRAPHENE_RECT_INIT((float) frame[0], (float) frame[1], (float) frame[2], (float) frame[3]);

    gtk_layout_manager_layout_changed(gtk_layout_child_get_layout_manager(GTK_LAYOUT_CHILD(layout_child)));
  }

  return result;
}

// A `graphene_matrix_t` is sixteen numbers in the order CSS gives `matrix3d`
// its own, which is the row-major order graphene reads them in.
static js_value_t *
bare_gtk_frame_layout_child_transform(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  BareFrameLayoutChild *layout_child;
  err = bare_gobject__read_type(env, argv[0], "layoutChild", BARE_TYPE_FRAME_LAYOUT_CHILD, (gpointer *) &layout_child);
  if (err < 0) return NULL;

  js_value_type_t type;
  err = js_typeof(env, argv[1], &type);
  assert(err == 0);

  if (type == js_null) {
    layout_child->transformed = false;
  } else {
    float components[16];

    for (uint32_t i = 0; i < 16; i++) {
      js_value_t *element;
      err = js_get_element(env, argv[1], i, &element);
      assert(err == 0);

      double component;
      err = bare_gtk__read_double(env, element, "transform", &component);
      if (err < 0) return NULL;

      components[i] = (float) component;
    }

    graphene_matrix_init_from_float(&layout_child->transform, components);

    layout_child->transformed = true;
  }

  gtk_layout_manager_layout_changed(gtk_layout_child_get_layout_manager(GTK_LAYOUT_CHILD(layout_child)));

  return NULL;
}

#endif // BARE_GTK_FRAME_LAYOUT_H
