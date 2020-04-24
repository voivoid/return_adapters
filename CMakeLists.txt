cmake_minimum_required(VERSION 3.17)

project(ReturnAdapters LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/thirdparties/callabletraits.cmake)
include(cmake/thirdparties/typestring.cmake)


# return_adapters

add_library(return_adapters INTERFACE)
target_link_libraries(return_adapters INTERFACE TypeString::lib CallableTraits::lib)
if(WIN32)
  target_compile_options(return_adapters INTERFACE /permissive-)
endif()

set(headers_dir inc/return_adapters)
target_sources(return_adapters INTERFACE ${headers_dir}/non_throwing_adapter.h
                                         ${headers_dir}/throwing_adapter.h
                                         ${headers_dir}/retval_adapter.h
                                         ${headers_dir}/out_retval_adapter.h
                                         ${headers_dir}/predicates.h
                                         ${headers_dir}/raii_adapter.h)

target_include_directories(return_adapters INTERFACE inc)


# tests

option(RETURN_ADAPTERS_BUILD_TESTS "Build unit tests" ON)
if(RETURN_ADAPTERS_BUILD_TESTS)
  include(cmake/thirdparties/catch2.cmake)

  enable_testing()
  add_subdirectory(tests)

  if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT return_adapters_tests)
  endif()
endif()


# examples

option(RETURN_ADAPTERS_BUILD_EXAMPLES "Build examples" ON)
if(RETURN_ADAPTERS_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

include(cmake/clangformat.cmake)
