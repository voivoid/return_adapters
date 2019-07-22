cmake_minimum_required(VERSION 3.8)

project(ReturnAdapters LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(return_adapters INTERFACE)

set(headers_dir ${CMAKE_CURRENT_SOURCE_DIR}/inc/return_adapters)
target_sources(return_adapters INTERFACE ${headers_dir}/non_throwing_adapter.h
                                         ${headers_dir}/throwing_adapter.h
                                         ${headers_dir}/retval_adapter.h)
target_include_directories(return_adapters INTERFACE inc)

option(RETURN_ADAPTERS_BUILD_TESTS "Build unit tests" ON)

if(RETURN_ADAPTERS_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)

  if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT return_adapters_tests)
  endif()

endif()


find_program(ClangFormat clang-format)
if(ClangFormat)
  get_target_property(LibSources return_adapters INTERFACE_SOURCES)
  get_target_property(TestSources return_adapters_tests SOURCES)

  add_custom_target(clangformat
    COMMAND clang-format -style=file -i ${LibSources} ${TestSources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
