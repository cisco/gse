add_executable(test_float test_float.cpp)

set_target_properties(test_float
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_link_libraries(test_float PRIVATE GTest::GTest GTest::Main)

add_test(NAME test_float
         COMMAND test_float)
