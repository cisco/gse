add_executable(test_half_float test_half_float.cpp)

set_target_properties(test_half_float
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_include_directories(test_half_float PRIVATE ${libgse_SOURCE_DIR}/src)

target_link_libraries(test_half_float PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_half_float
         COMMAND test_half_float)
