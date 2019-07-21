include(ExternalProject)

find_package(Git REQUIRED)

set(Catch2Ver v2.9.1)

ExternalProject_Add(
  get_catch2
  PREFIX thirdparties
  URL https://github.com/catchorg/Catch2/archive/${Catch2Ver}.zip
  URL_HASH SHA256=9a79099873c8b3156a8f7937d7976ef85bf35f46e3f0a963117be44bc9bbc5f8
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  )

add_library(catch2 INTERFACE)
add_library(Catch::v2 ALIAS catch2)

ExternalProject_Get_Property(get_catch2 SOURCE_DIR)
set_target_properties(catch2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/single_include)
set_target_properties(catch2 PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${SOURCE_DIR}/single_include)

add_dependencies(catch2 get_catch2)
