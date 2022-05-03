add_executable(test_gs_decoder test_main.cpp test_gs_decoder.cpp)

include(GoogleTest)

target_link_libraries(test_gs_decoder PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_gs_decoder
         COMMAND test_gs_decoder)
