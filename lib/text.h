#ifndef BARE_GTK_TEXT_H
#define BARE_GTK_TEXT_H

#include <assert.h>
#include <js.h>
#include <math.h>

#include <gtk/gtk.h>

#include "bridging.h"

static PangoContext *
bare_gtk_text__context(bare_gtk_state_t *state) {
  if (state->text_probe) return gtk_widget_get_pango_context(state->text_probe);

  state->text_probe = g_object_ref_sink(gtk_label_new("Mg"));

  PangoContext *context = gtk_widget_get_pango_context(state->text_probe);

  PangoLayout *layout = pango_layout_new(context);
  pango_layout_set_text(layout, "Mg", -1);

  int width, height;
  pango_layout_get_pixel_size(layout, &width, &height);

  g_object_unref(layout);

  int minimum, natural;

  gtk_widget_measure(state->text_probe, GTK_ORIENTATION_HORIZONTAL, -1, &minimum, &natural, NULL, NULL);
  state->text_inset_width = natural - width;

  gtk_widget_measure(state->text_probe, GTK_ORIENTATION_VERTICAL, -1, &minimum, &natural, NULL, NULL);
  state->text_inset_height = natural - height;

  return context;
}

static js_value_t *
bare_gtk_text_measure(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 4);

  g_autofree char *text = NULL;
  err = bare_gtk__read_string(env, argv[0], "text", &text);
  if (err < 0) return NULL;

  g_autofree char *family = NULL;
  err = bare_gtk__read_string_or_null(env, argv[1], "family", &family);
  if (err < 0) return NULL;

  double size;
  err = bare_gtk__read_double(env, argv[2], "size", &size);
  if (err < 0) return NULL;

  double available;
  err = bare_gtk__read_double(env, argv[3], "width", &available);
  if (err < 0) return NULL;

  PangoContext *context = bare_gtk_text__context(state);

  PangoLayout *layout = pango_layout_new(context);

  pango_layout_set_text(layout, text, -1);

  if (family || size > 0) {
    PangoFontDescription *font = pango_font_description_copy(pango_context_get_font_description(context));

    if (family) pango_font_description_set_family(font, family);

    // Absolute, so the size is in device pixels and not scaled by the context
    // resolution. A label matches this with a CSS font size in px.
    if (size > 0) pango_font_description_set_absolute_size(font, size * PANGO_SCALE);

    pango_layout_set_font_description(layout, font);

    pango_font_description_free(font);
  }

  pango_layout_set_wrap(layout, PANGO_WRAP_WORD_CHAR);

  pango_layout_set_width(
    layout,
    isfinite(available) && available >= 0 && available < (double) (G_MAXINT / PANGO_SCALE)
      ? (int) (available * PANGO_SCALE)
      : -1
  );

  int width, height;
  pango_layout_get_pixel_size(layout, &width, &height);

  int lines = pango_layout_get_line_count(layout);

  g_object_unref(layout);

  js_value_t *result;
  err = js_create_array_with_length(env, 3, &result);
  assert(err == 0);

#define V(i, d) \
  { \
    js_value_t *val; \
    err = js_create_double(env, d, &val); \
    assert(err == 0); \
    err = js_set_element(env, result, i, val); \
    assert(err == 0); \
  }
  V(0, width + state->text_inset_width)
  V(1, height + state->text_inset_height)
  V(2, lines)
#undef V

  return result;
}

// Measured from the same markup a label is given, so both use one font
// description.
static js_value_t *
bare_gtk_text_measure_markup(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_gtk_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  g_autofree char *markup = NULL;
  err = bare_gtk__read_string(env, argv[0], "markup", &markup);
  if (err < 0) return NULL;

  double available;
  err = bare_gtk__read_double(env, argv[1], "width", &available);
  if (err < 0) return NULL;

  PangoContext *context = bare_gtk_text__context(state);

  PangoLayout *layout = pango_layout_new(context);

  pango_layout_set_markup(layout, markup, -1);

  pango_layout_set_wrap(layout, PANGO_WRAP_WORD_CHAR);

  pango_layout_set_width(
    layout,
    isfinite(available) && available >= 0 && available < (double) (G_MAXINT / PANGO_SCALE)
      ? (int) (available * PANGO_SCALE)
      : -1
  );

  int width, height;
  pango_layout_get_pixel_size(layout, &width, &height);

  int lines = pango_layout_get_line_count(layout);

  g_object_unref(layout);

  js_value_t *result;
  err = js_create_array_with_length(env, 3, &result);
  assert(err == 0);

#define V(i, d) \
  { \
    js_value_t *val; \
    err = js_create_double(env, d, &val); \
    assert(err == 0); \
    err = js_set_element(env, result, i, val); \
    assert(err == 0); \
  }
  V(0, width + state->text_inset_width)
  V(1, height + state->text_inset_height)
  V(2, lines)
#undef V

  return result;
}

#endif // BARE_GTK_TEXT_H
