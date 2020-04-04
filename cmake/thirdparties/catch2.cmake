include(ExternalProject)

find_package(Git REQUIRED)

set(Catch2Ver v2.11.3)

ExternalProject_Add(
  get_catch2
  PREFIX thirdparties
  URL https://github.com/catchorg/Catch2/archive/${Catch2Ver}.zip
  URL_HASH SHA256=c5a0a7510379c6f37f70b329986a335a7b8489d67ac417ce8f4262d0cae4cc5d
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
