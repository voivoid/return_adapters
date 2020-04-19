# clang-format

find_program(ClangFormat clang-format)
if(ClangFormat)
  get_target_property(LibSources return_adapters INTERFACE_SOURCES)

  if(RETURN_ADAPTERS_BUILD_TESTS)
    get_target_property(TestsSources return_adapters_tests SOURCES)
  endif()

  if(RETURN_ADAPTERS_BUILD_EXAMPLES)
    get_target_property(ExamplesSources return_adapters_examples SOURCES)
  endif()

  add_custom_target(clangformat
    COMMAND clang-format -style=file -i ${LibSources} ${TestsSources} ${ExamplesSources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()