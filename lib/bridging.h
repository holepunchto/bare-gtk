#ifndef BARE_GTK_BRIDGING_H
#define BARE_GTK_BRIDGING_H

#include <assert.h>
#include <glib.h>
#include <js.h>
#include <stdbool.h>
#include <stdint.h>
#include <utf.h>

#include "registry.h"

static int
bare_gtk__read_bool(js_env_t *env, js_value_t *value, const char *name, bool *result) {
  int err;

  bool is;
  err = js_is_boolean(env, value, &is);
  assert(err == 0);

  if (!is) {
    err = js_throw_type_errorf(env, NULL, "Expected '%s' to be a boolean", name);
    assert(err == 0);

    return -1;
  }

  err = js_get_value_bool(env, value, result);
  assert(err == 0);

  return 0;
}

static int
bare_gtk__read_number(js_env_t *env, js_value_t *value, const char *name) {
  int err;

  bool is;
  err = js_is_number(env, value, &is);
  assert(err == 0);

  if (!is) {
    err = js_throw_type_errorf(env, NULL, "Expected '%s' to be a number", name);
    assert(err == 0);

    return -1;
  }

  return 0;
}

static int
bare_gtk__read_int32(js_env_t *env, js_value_t *value, const char *name, int32_t *result) {
  int err;

  err = bare_gtk__read_number(env, value, name);
  if (err < 0) return err;

  err = js_get_value_int32(env, value, result);
  assert(err == 0);

  return 0;
}

static int
bare_gtk__read_uint32(js_env_t *env, js_value_t *value, const char *name, uint32_t *result) {
  int err;

  err = bare_gtk__read_number(env, value, name);
  if (err < 0) return err;

  err = js_get_value_uint32(env, value, result);
  assert(err == 0);

  return 0;
}

static int
bare_gtk__read_double(js_env_t *env, js_value_t *value, const char *name, double *result) {
  int err;

  err = bare_gtk__read_number(env, value, name);
  if (err < 0) return err;

  err = js_get_value_double(env, value, result);
  assert(err == 0);

  return 0;
}

static int
bare_gtk__read_string(js_env_t *env, js_value_t *value, const char *name, char **result) {
  int err;

  bool is;
  err = js_is_string(env, value, &is);
  assert(err == 0);

  if (!is) {
    err = js_throw_type_errorf(env, NULL, "Expected '%s' to be a string", name);
    assert(err == 0);

    return -1;
  }

  size_t len;
  err = js_get_value_string_utf8(env, value, NULL, 0, &len);
  assert(err == 0);

  char *str = g_malloc(len + 1);

  err = js_get_value_string_utf8(env, value, (utf8_t *) str, len + 1, NULL);
  assert(err == 0);

  *result = str;

  return 0;
}

static int
bare_gtk__read_string_or_null(js_env_t *env, js_value_t *value, const char *name, char **result) {
  int err;

  bool is;
  err = js_is_null(env, value, &is);
  assert(err == 0);

  if (is) {
    *result = NULL;

    return 0;
  }

  return bare_gtk__read_string(env, value, name, result);
}

static int
bare_gtk__read_tag_or_null(js_env_t *env, js_value_t *value, const char *name, gpointer *result) {
  int err;

  bool is;
  err = js_is_null(env, value, &is);
  assert(err == 0);

  if (is) {
    *result = NULL;

    return 0;
  }

  return bare_gobject__read_tag(env, value, name, result);
}

static js_value_t *
bare_gtk__create_tag(js_env_t *env, gpointer object) {
  int err;

  js_value_t *result;

  if (object) {
    err = js_create_uint32(env, bare_gobject__tag(object), &result);
  } else {
    err = js_get_null(env, &result);
  }

  assert(err == 0);

  return result;
}

static void
bare_gtk__emit(js_env_t *env, gpointer object, const char *event, size_t argc, const double args[]) {
  int err;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *wrapper = bare_gobject__wrapper(env, object);

  if (wrapper) {
    js_value_t *emit;
    err = js_get_named_property(env, wrapper, "emit", &emit);
    assert(err == 0);

    js_value_t *argv[8];

    assert(argc + 1 <= 8);

    err = js_create_string_utf8(env, (const utf8_t *) event, (size_t) -1, &argv[0]);
    assert(err == 0);

    for (size_t i = 0; i < argc; i++) {
      err = js_create_double(env, args[i], &argv[i + 1]);
      assert(err == 0);
    }

    err = js_call_function(env, wrapper, emit, argc + 1, argv, NULL);
    assert(err == 0 || err == js_pending_exception);
  }

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

#endif // BARE_GTK_BRIDGING_H
