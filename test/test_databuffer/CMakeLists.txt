add_executable(test_databuffer test_databuffer.cpp)

set_target_properties(test_databuffer
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_link_libraries(test_databuffer PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_databuffer
         COMMAND test_databuffer)
