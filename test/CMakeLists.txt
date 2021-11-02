add_executable(io_buffer_test.cpp io_buffer_test)

target_link_libraries(
        io_buffer_test
        gtest_main ${PROJECT_NAME}
)

include(GoogleTest)
gtest_discover_tests(io_buffer_test)
