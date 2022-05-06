add_library(doctest OBJECT doctest.cpp)

add_executable(json_test json_test.cpp)
target_link_libraries(json_test PRIVATE doctest)

add_executable(file_reader_test file_reader_test.cpp)
target_link_libraries(file_reader_test PRIVATE doctest)

add_test(NAME json_test COMMAND json_test)
add_test(NAME file_reader_test COMMAND file_reader_test)