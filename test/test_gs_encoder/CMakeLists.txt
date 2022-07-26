add_executable(test_gs_encoder test_gs_encoder.cpp)

set_target_properties(test_gs_encoder
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

include(GoogleTest)

target_link_libraries(test_gs_encoder PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_gs_encoder
         COMMAND test_gs_encoder)
