#ifndef BARE_GTK_LAYOUT_MANAGER_H
#define BARE_GTK_LAYOUT_MANAGER_H

#include <assert.h>
#include <js.h>

#include <gtk/gtk.h>

#include "bridging.h"

static js_value_t *
bare_gtk_layout_manager_layout_child(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  GtkLayoutManager *manager;
  err = bare_gobject__read_tag(env, argv[0], "layoutManager", (gpointer *) &manager);
  if (err < 0) return NULL;

  GtkWidget *child;
  err = bare_gobject__read_tag(env, argv[1], "child", (gpointer *) &child);
  if (err < 0) return NULL;

  if (gtk_widget_get_parent(child) == NULL) {
    err = js_throw_error(env, NULL, "Widget has no parent");
    assert(err == 0);

    return NULL;
  }

  return bare_gtk__create_tag(env, gtk_layout_manager_get_layout_child(manager, child));
}

#endif // BARE_GTK_LAYOUT_MANAGER_H
