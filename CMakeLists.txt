cmake_minimum_required(VERSION 4.0)

find_package(cmake-bare REQUIRED PATHS node_modules/cmake-bare)
find_package(cmake-fetch REQUIRED PATHS node_modules/cmake-fetch)

find_package(PkgConfig REQUIRED)

project(bare_gtk C)

fetch_package("github:holepunchto/bare@1.29.4")
fetch_package("github:holepunchto/libpath")

pkg_check_modules(GTK REQUIRED IMPORTED_TARGET GLOBAL gtk4>=4.6)

# The version the prebuilds are built against.
target_compile_definitions(
  PkgConfig::GTK
  INTERFACE
    GDK_VERSION_MIN_REQUIRED=GDK_VERSION_4_6
    GDK_VERSION_MAX_ALLOWED=GDK_VERSION_4_6
)

add_bare_module(bare_gtk)

target_sources(
  ${bare_gtk}
  PRIVATE
    binding.c
)

target_include_directories(
  ${bare_gtk}
  PRIVATE
    node_modules/bare-gobject-registry/lib
)

target_link_libraries(
  ${bare_gtk}
  PUBLIC
    PkgConfig::GTK
)

add_executable(bare_runtime)

target_sources(
  bare_runtime
  PRIVATE
    lib/runtime.c
)

set(bare_runtime_rpath "$ORIGIN/../lib")

set_target_properties(
  bare_runtime
  PROPERTIES
  C_STANDARD 99
  POSITION_INDEPENDENT_CODE ON
  OUTPUT_NAME bare
  INSTALL_RPATH "${bare_runtime_rpath}"
  BUILD_WITH_INSTALL_RPATH ON
  ENABLE_EXPORTS ON
)

target_link_libraries(
  bare_runtime
  PUBLIC
    $<LINK_LIBRARY:WHOLE_ARCHIVE,bare_static>
  PRIVATE
    rlimit_static
    path_static
  PRIVATE
    PkgConfig::GTK
)

bare_target(target)

install(TARGETS bare_runtime RUNTIME DESTINATION ${target})
