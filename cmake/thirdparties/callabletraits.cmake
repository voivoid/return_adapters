include(ExternalProject)

set(CallableTraitsVer 2.3.2)

ExternalProject_Add(
  get_callabletraits
  PREFIX thirdparties
  URL https://github.com/boostorg/callable_traits/archive/${CallableTraitsVer}.zip
  URL_HASH SHA256=f5037c6445cea8992def322633b1c0c6b9e20c819f3c7927aa6d393881415850
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  )

add_library(callabletraits INTERFACE)
add_library(CallableTraits::lib ALIAS callabletraits)

ExternalProject_Get_Property(get_callabletraits SOURCE_DIR)
set_target_properties(callabletraits PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)
set_target_properties(callabletraits PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)

add_dependencies(callabletraits get_callabletraits)
