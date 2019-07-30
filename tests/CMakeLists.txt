set(tests_dir ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(return_adapters_tests ${tests_dir}/main.cpp
                                     ${tests_dir}/non_throwing_adapter.cpp
                                     ${tests_dir}/throwing_adapter.cpp
                                     ${tests_dir}/retval_adapter.cpp
                                     ${tests_dir}/safe_crt.cpp)

if(WIN32)
  target_sources(return_adapters_tests PRIVATE ${tests_dir}/safe_com.cpp)
  target_compile_definitions(return_adapters_tests PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()


target_link_libraries(return_adapters_tests PRIVATE return_adapters Catch::v2)

add_test(NAME return_adapters_tests
         COMMAND $<TARGET_FILE:return_adapters_tests>)
