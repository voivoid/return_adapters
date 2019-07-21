include(catch2.cmake)

add_executable(return_adapters_tests main.cpp non_throwing_adapter.cpp)
target_link_libraries(return_adapters_tests PRIVATE return_adapters Catch::v2)

add_test(NAME return_adapters_tests
         COMMAND $<TARGET_FILE:return_adapters_tests>)