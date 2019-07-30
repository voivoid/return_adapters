include(ExternalProject)

find_package(Git REQUIRED)

set(FmtVer 5.3.0)

ExternalProject_Add(
  get_fmt
  PREFIX thirdparties
  URL https://github.com/fmtlib/fmt/archive/${FmtVer}.zip
  URL_HASH SHA256=2ba1469c1f7677e70cb32bcd7dd0907e6952e16a161f07e05c7ffa388643b607
  CMAKE_ARGS -DFMT_TEST=OFF -DFMT_INSTALL=OFF -DFMT_DOC=OFF
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  )

add_library(Fmt::Fmt STATIC IMPORTED GLOBAL)

ExternalProject_Get_Property(get_fmt SOURCE_DIR)
set_target_properties(Fmt::Fmt PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)
set_target_properties(Fmt::Fmt PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)

ExternalProject_Get_Property(get_fmt BINARY_DIR)

if(MSVC)
  set_property(TARGET Fmt::Fmt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
  set_property(TARGET Fmt::Fmt APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)

  set_target_properties(Fmt::Fmt PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
    IMPORTED_LOCATION_RELEASE "${BINARY_DIR}/release/fmt.lib"
    IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
    IMPORTED_LOCATION_DEBUG "${BINARY_DIR}/debug/fmtd.lib")
else()
  set_target_properties(Fmt::Fmt PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
      IMPORTED_LOCATION "${BINARY_DIR}/libfmt.a")
endif()

add_dependencies(Fmt::Fmt get_fmt)
