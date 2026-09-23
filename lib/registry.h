#ifndef BARE_GOBJECT_REGISTRY_H
#define BARE_GOBJECT_REGISTRY_H

#include <assert.h>
#include <glib-object.h>
#include <js.h>
#include <stdint.h>

// Tags a GObject, holds one reference to it, and keeps a weak reference back to
// its wrapper so that an accessor can hand back the wrapper it already has.

static const js_type_tag_t bare_gobject__carrier = {0x8f1d3a6c5e204b97, 0xb4e70c9218d3a65f};

typedef struct {
  GObject *object;
  js_ref_t *wrapper;
} bare_gobject_entry_t;

static GHashTable *bare_gobject__entries;
static GHashTable *bare_gobject__tags;
static uint32_t bare_gobject__next_tag = 1;

static void
bare_gobject__registry_init(void) {
  if (bare_gobject__entries) return;

  bare_gobject__entries = g_hash_table_new_full(NULL, NULL, NULL, g_free);
  bare_gobject__tags = g_hash_table_new(NULL, NULL);
}

static uint32_t
bare_gobject__tag(gpointer object) {
  bare_gobject__registry_init();

  gpointer existing;

  if (g_hash_table_lookup_extended(bare_gobject__tags, object, NULL, &existing)) {
    return GPOINTER_TO_UINT(existing);
  }

  uint32_t tag = bare_gobject__next_tag++;

  bare_gobject_entry_t *entry = g_new0(bare_gobject_entry_t, 1);

  entry->object = g_object_ref_sink(object);

  g_hash_table_insert(bare_gobject__entries, GUINT_TO_POINTER(tag), entry);
  g_hash_table_insert(bare_gobject__tags, object, GUINT_TO_POINTER(tag));

  return tag;
}

static bare_gobject_entry_t *
bare_gobject__entry(uint32_t tag) {
  bare_gobject__registry_init();

  return g_hash_table_lookup(bare_gobject__entries, GUINT_TO_POINTER(tag));
}

static int
bare_gobject__read_uint32(js_env_t *env, js_value_t *value, const char *name, uint32_t *result) {
  int err;

  bool is;
  err = js_is_number(env, value, &is);
  assert(err == 0);

  if (!is) {
    err = js_throw_type_errorf(env, NULL, "Expected '%s' to be a number", name);
    assert(err == 0);

    return -1;
  }

  err = js_get_value_uint32(env, value, result);
  assert(err == 0);

  return 0;
}

static int
bare_gobject__read_tag(js_env_t *env, js_value_t *value, const char *name, gpointer *result) {
  int err;

  uint32_t tag;
  err = bare_gobject__read_uint32(env, value, name, &tag);
  if (err < 0) return err;

  bare_gobject_entry_t *entry = bare_gobject__entry(tag);

  if (entry == NULL) {
    err = js_throw_errorf(env, NULL, "Unknown tag %u", tag);
    assert(err == 0);

    return -1;
  }

  *result = entry->object;

  return 0;
}

static int
bare_gobject__read_type(js_env_t *env, js_value_t *value, const char *name, GType type, gpointer *result) {
  int err;

  gpointer object;
  err = bare_gobject__read_tag(env, value, name, &object);
  if (err < 0) return err;

  if (!G_TYPE_CHECK_INSTANCE_TYPE(object, type)) {
    err = js_throw_type_errorf(env, NULL, "Expected '%s' to be a %s, not a %s", name, g_type_name(type), G_OBJECT_TYPE_NAME(object));
    assert(err == 0);

    return -1;
  }

  *result = object;

  return 0;
}

static js_value_t *
bare_gobject__wrapper(js_env_t *env, gpointer object) {
  int err;

  bare_gobject__registry_init();

  gpointer tag;

  if (!g_hash_table_lookup_extended(bare_gobject__tags, object, NULL, &tag)) return NULL;

  bare_gobject_entry_t *entry = g_hash_table_lookup(bare_gobject__entries, tag);

  if (entry == NULL || entry->wrapper == NULL) return NULL;

  js_value_t *result;
  err = js_get_reference_value(env, entry->wrapper, &result);
  assert(err == 0);

  return result;
}

static void
bare_gobject__on_token_finalize(js_env_t *env, void *data, void *finalize_hint) {
  int err;

  uint32_t tag = (uint32_t) (uintptr_t) data;

  bare_gobject_entry_t *entry = bare_gobject__entry(tag);

  if (entry == NULL) return;

  if (entry->wrapper) {
    err = js_delete_reference(env, entry->wrapper);
    assert(err == 0);
  }

  GObject *object = entry->object;

  g_hash_table_remove(bare_gobject__tags, object);
  g_hash_table_remove(bare_gobject__entries, GUINT_TO_POINTER(tag));

  g_object_unref(object);
}

static js_value_t *
bare_gobject_registry_claim(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  uint32_t tag;
  err = bare_gobject__read_uint32(env, argv[0], "tag", &tag);
  if (err < 0) return NULL;

  bare_gobject_entry_t *entry = bare_gobject__entry(tag);

  if (entry == NULL) {
    err = js_throw_errorf(env, NULL, "Unknown tag %u", tag);
    assert(err == 0);

    return NULL;
  }

  err = js_create_reference(env, argv[1], 0, &entry->wrapper);
  assert(err == 0);

  js_value_t *token;
  err = js_create_external(env, (void *) (uintptr_t) tag, bare_gobject__on_token_finalize, NULL, &token);
  assert(err == 0);

  return token;
}

static js_value_t *
bare_gobject_registry_wrapper(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  gpointer object;
  err = bare_gobject__read_tag(env, argv[0], "tag", &object);
  if (err < 0) return NULL;

  js_value_t *result = bare_gobject__wrapper(env, object);

  if (result == NULL) {
    err = js_get_null(env, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_gobject_registry_size(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_gobject__registry_init();

  js_value_t *result;
  err = js_create_uint32(env, g_hash_table_size(bare_gobject__entries), &result);
  assert(err == 0);

  return result;
}

static void
bare_gobject__on_carrier_finalize(js_env_t *env, void *data, void *finalize_hint) {
  g_object_unref(data);
}

static js_value_t *
bare_gobject_registry_handle(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  gpointer object;
  err = bare_gobject__read_tag(env, argv[0], "tag", &object);
  if (err < 0) return NULL;

  js_value_t *carrier;
  err = js_create_object(env, &carrier);
  assert(err == 0);

  err = js_wrap(env, carrier, g_object_ref(object), bare_gobject__on_carrier_finalize, NULL, NULL);
  assert(err == 0);

  err = js_add_type_tag(env, carrier, &bare_gobject__carrier);
  assert(err == 0);

  return carrier;
}

static js_value_t *
bare_gobject_registry_adopt(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  bool is;
  err = js_is_object(env, argv[0], &is);
  assert(err == 0);

  if (is) {
    err = js_check_type_tag(env, argv[0], &bare_gobject__carrier, &is);
    assert(err == 0);
  }

  if (!is) {
    err = js_throw_type_error(env, NULL, "Expected 'carrier' to be a GObject handle");
    assert(err == 0);

    return NULL;
  }

  gpointer object;
  err = js_unwrap(env, argv[0], &object);
  assert(err == 0);

  js_value_t *result;
  err = js_create_uint32(env, bare_gobject__tag(object), &result);
  assert(err == 0);

  return result;
}

#endif // BARE_GOBJECT_REGISTRY_H
