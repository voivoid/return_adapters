cmake_minimum_required(VERSION 3.0)

project(ReturnAdapters LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(return_adapters INTERFACE)

set(headers_dir ${CMAKE_CURRENT_SOURCE_DIR}/inc/return_adapters)
target_sources(return_adapters INTERFACE ${headers_dir}/non_throwing_adapter.h
                                         ${headers_dir}/throwing_adapter.h)
target_include_directories(return_adapters INTERFACE inc)

option(RETURN_ADAPTERS_BUILD_TESTS "Build unit tests" ON)

if(RETURN_ADAPTERS_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
