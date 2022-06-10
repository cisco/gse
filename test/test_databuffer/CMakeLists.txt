add_executable(test_databuffer test_databuffer.cpp)

include(GoogleTest)

target_link_libraries(test_databuffer PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_databuffer
         COMMAND test_databuffer)
