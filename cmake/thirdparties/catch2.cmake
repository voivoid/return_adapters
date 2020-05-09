include(ExternalProject)

find_package(Git REQUIRED)

set(Catch2Ver v2.12.1)

ExternalProject_Add(
  get_catch2
  PREFIX thirdparties
  URL https://github.com/catchorg/Catch2/archive/${Catch2Ver}.zip
  URL_HASH SHA256=cd139306f63404df38bf8413c4cb284aa9c23c917044c9a1e170f1854626a502
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  )

add_library(catch2 INTERFACE)
add_library(Catch2::lib ALIAS catch2)

ExternalProject_Get_Property(get_catch2 SOURCE_DIR)
set_target_properties(catch2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/single_include)
set_target_properties(catch2 PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${SOURCE_DIR}/single_include)

add_dependencies(catch2 get_catch2)
