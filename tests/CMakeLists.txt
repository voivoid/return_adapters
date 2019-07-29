set(tests_dir ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(return_adapters_tests ${tests_dir}/main.cpp
                                     ${tests_dir}/non_throwing_adapter.cpp
                                     ${tests_dir}/throwing_adapter.cpp
                                     ${tests_dir}/retval_adapter.cpp)

if(WIN32)
  target_sources(return_adapters_tests PRIVATE ${tests_dir}/com_adapters.cpp)
endif()


target_link_libraries(return_adapters_tests PRIVATE return_adapters Catch::v2)

add_test(NAME return_adapters_tests
         COMMAND $<TARGET_FILE:return_adapters_tests>)
