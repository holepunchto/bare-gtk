#include <assert.h>
#include <bare.h>
#include <js.h>
#include <log.h>
#include <path.h>
#include <rlimit.h>
#include <signal.h>
#include <uv.h>

#include <gtk/gtk.h>

static uv_barrier_t bare__platform_ready;
static uv_async_t bare__platform_shutdown;
static uv_thread_t bare__platform_thread;
static js_platform_t *bare__platform;

static int bare__argc;
static char **bare__argv;

static uv_loop_t *bare__loop;
static uv_async_t bare__shutdown;
static bare_t *bare;

static GIOChannel *bare__channel;
static guint bare__poll;
static guint bare__timer;

static void
bare__on_shutdown(uv_async_t *handle) {
  uv_close((uv_handle_t *) handle, NULL);
}

static void
bare__on_platform_thread(void *data) {
  int err;

  uv_loop_t loop;
  err = uv_loop_init(&loop);
  assert(err == 0);

  err = uv_async_init(&loop, &bare__platform_shutdown, bare__on_shutdown);
  assert(err == 0);

  err = js_create_platform(&loop, NULL, &bare__platform);
  assert(err == 0);

  uv_barrier_wait(&bare__platform_ready);

  err = uv_run(&loop, UV_RUN_DEFAULT);
  assert(err == 0);

  err = js_destroy_platform(bare__platform);
  assert(err == 0);

  err = uv_run(&loop, UV_RUN_DEFAULT);
  assert(err == 0);

  err = uv_loop_close(&loop);
  assert(err == 0);
}

static void
bare__run(void);

static gboolean
bare__on_poll(GIOChannel *source, GIOCondition condition, gpointer data) {
  bare__run();

  return G_SOURCE_CONTINUE;
}

static gboolean
bare__on_timeout(gpointer data) {
  bare__timer = 0;

  bare__run();

  return G_SOURCE_REMOVE;
}

static void
bare__run(void) {
  int err;

  if (bare__timer) {
    g_source_remove(bare__timer);

    bare__timer = 0;
  }

  int timeout = uv_backend_timeout(bare__loop);

  if (timeout == 0) {
    bare__timer = g_idle_add(bare__on_timeout, NULL);
  } else if (timeout > 0) {
    bare__timer = g_timeout_add(timeout, bare__on_timeout, NULL);
  }

  err = bare_run(bare, UV_RUN_NOWAIT);
  assert(err >= 0);
}

static void
bare__on_activate(GtkApplication *app, gpointer data) {
  int err;

  err = uv_barrier_init(&bare__platform_ready, 2);
  assert(err == 0);

  err = uv_thread_create(&bare__platform_thread, bare__on_platform_thread, NULL);
  assert(err == 0);

  uv_barrier_wait(&bare__platform_ready);

  uv_barrier_destroy(&bare__platform_ready);

  bare__loop = uv_default_loop();

  err = uv_async_init(bare__loop, &bare__shutdown, bare__on_shutdown);
  assert(err == 0);

  err = bare_setup(bare__loop, bare__platform, NULL, bare__argc, (const char **) bare__argv, NULL, &bare);
  assert(err == 0);

  size_t len;

  char bin[4096];
  len = sizeof(bin);

  err = uv_exepath(bin, &len);
  assert(err == 0);

  size_t dir;
  err = path_dirname(bin, &dir, path_behavior_system);
  assert(err == 0);

  char bundle[4096];
  len = 4096;

  err = path_join(
    (const char *[]) {bin, "..", "..", "share", &bin[dir], "app.bundle", NULL},
    bundle,
    &len,
    path_behavior_system
  );
  assert(err == 0);

  err = bare_load(bare, bundle, NULL, NULL);
  (void) err;

  bare__channel = g_io_channel_unix_new(bare__loop->backend_fd);

  bare__poll = g_io_add_watch(bare__channel, G_IO_IN, bare__on_poll, NULL);

  bare__timer = 0;

  bare__run();
}

int
main(int argc, char *argv[]) {
  int err;

#ifdef SIGPIPE
  signal(SIGPIPE, SIG_IGN);
#endif

  err = log_open("bare", 0);
  assert(err == 0);

  err = rlimit_set(rlimit_open_files, rlimit_infer);
  assert(err == 0);

  argv = uv_setup_args(argc, argv);

  bare__argc = argc;
  bare__argv = argv;

  GtkApplication *app = gtk_application_new(NULL, 0);

  g_signal_connect(app, "activate", G_CALLBACK(bare__on_activate), NULL);

  g_application_run(G_APPLICATION(app), argc, argv);

  g_source_remove(bare__poll);

  if (bare__timer) g_source_remove(bare__timer);

  g_io_channel_unref(bare__channel);

  err = uv_async_send(&bare__shutdown);
  assert(err == 0);

  err = bare_terminate(bare);
  assert(err == 0);

  err = bare_run(bare, UV_RUN_DEFAULT);
  assert(err == 0);

  int exit_code;
  err = bare_teardown(bare, UV_RUN_DEFAULT, &exit_code);
  assert(err == 0);

  err = uv_loop_close(bare__loop);
  assert(err == 0);

  err = uv_async_send(&bare__platform_shutdown);
  assert(err == 0);

  uv_thread_join(&bare__platform_thread);

  err = log_close();
  assert(err == 0);

  return exit_code;
}
