add_executable(test_gs_decoder test_gs_decoder.cpp)

set_target_properties(test_gs_decoder
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_link_libraries(test_gs_decoder PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_gs_decoder
         COMMAND test_gs_decoder)
