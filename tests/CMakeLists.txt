add_executable(return_adapters_tests main.cpp
                                     exception_adapter.cpp
                                     non_throwing_adapter.cpp
                                     throwing_adapter.cpp
                                     retval_adapter.cpp
                                     out_retval_adapter.cpp
                                     raii_adapter.cpp
                                     test_utils.h)


target_link_libraries(return_adapters_tests PRIVATE return_adapters Catch2::lib)

add_test(NAME return_adapters_tests
         COMMAND $<TARGET_FILE:return_adapters_tests>)
