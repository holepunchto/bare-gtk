cmake_minimum_required(VERSION 4.0)

find_package(cmake-bare REQUIRED PATHS node_modules/cmake-bare)
find_package(cmake-fetch REQUIRED PATHS node_modules/cmake-fetch)
find_package(cmake-prelude REQUIRED PATHS node_modules/cmake-prelude)

find_package(PkgConfig REQUIRED)

project(bare_gtk C)

fetch_package("github:holepunchto/bare@1.38.0")
fetch_package("github:holepunchto/libpath#a68d099")

pkg_check_modules(GTK REQUIRED IMPORTED_TARGET GLOBAL gtk4>=4.6)

target_compile_definitions(
  PkgConfig::GTK
  INTERFACE
    GDK_VERSION_MIN_REQUIRED=GDK_VERSION_4_6
    GDK_VERSION_MAX_ALLOWED=GDK_VERSION_4_6
)

add_bare_module(
  bare_gtk
  SOURCES
    binding.c
  LINK
    PkgConfig::GTK
)

target_include_directories(
  ${bare_gtk}
  PRIVATE
    node_modules/bare-gobject-registry/lib
)

bare_target(target)

add_executable_target(
  bare_runtime
  SOURCES
    lib/runtime.c
  LINK
    $<LINK_LIBRARY:WHOLE_ARCHIVE,bare::static>
    bare::exports
    rlimit::static
    path::static
    PkgConfig::GTK
  EXPORTS
    bare_gtk_app
  RPATH ../lib
  DESTINATION ${target}
  OUTPUT_DIRECTORY bin
  EXPORT OFF
)

set_target_properties(
  bare_runtime_executable
  PROPERTIES
  OUTPUT_NAME bare
)
