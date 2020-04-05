include(ExternalProject)

find_package(Git REQUIRED)

set(TypeStringVer master)

ExternalProject_Add(
  get_typestring
  PREFIX thirdparties
  URL https://github.com/irrequietus/typestring/archive/${TypeStringVer}.zip
  URL_HASH SHA256=2b21bbd470809a7701849964a00ad0bf875eba58c67bf546c2ec26fc8d17ef4f
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  )

add_library(typestring INTERFACE)
add_library(TypeString::lib ALIAS typestring)

ExternalProject_Get_Property(get_typestring SOURCE_DIR)
set_target_properties(typestring PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR})
set_target_properties(typestring PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${SOURCE_DIR})

add_dependencies(typestring get_typestring)
