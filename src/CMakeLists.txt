add_library(doctest OBJECT doctest.cpp)

add_executable(json_test json_test.cpp)
target_link_libraries(json_test PRIVATE doctest)

add_executable(file_reader_test file_reader_test.cpp)
target_link_libraries(file_reader_test PRIVATE doctest)

add_executable(stochastic_oscillator_test stochastic_oscillator_test.cpp)
target_link_libraries(stochastic_oscillator_test PRIVATE doctest)

add_executable(trading_test trading_test.cpp)
target_link_libraries(trading_test PRIVATE doctest)

add_executable(concurrent_trading_test concurrent_trading_test.cpp)
target_link_libraries(concurrent_trading_test PRIVATE doctest)

add_executable(representation_test representation_test.cpp)
if(Qt5_FOUND)
    target_link_libraries(representation_test PRIVATE Qt5::Charts Qt5::Widgets Qt5::Core doctest)
else(Qt5_FOUND)
    target_link_libraries(representation_test PRIVATE doctest)
endif(Qt5_FOUND)
add_test(NAME representation_test COMMAND representation_test)

add_test(NAME json_test COMMAND json_test)
add_test(NAME file_reader_test COMMAND file_reader_test)
add_test(NAME stochastic_oscillator_test COMMAND stochastic_oscillator_test)
add_test(NAME trading_test COMMAND trading_test)
add_test(NAME concurrent_trading_test COMMAND concurrent_trading_test)