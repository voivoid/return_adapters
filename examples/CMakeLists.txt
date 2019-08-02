set(examples_dir ${CMAKE_CURRENT_SOURCE_DIR})
add_library(return_adapters_examples STATIC ${examples_dir}/safe_crt.cpp)

if(WIN32)
  target_sources(return_adapters_examples PUBLIC ${examples_dir}/safe_com.cpp)

  target_compile_definitions(return_adapters_examples PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(return_adapters_examples PRIVATE return_adapters)
