add_executable(test_gs_encoder test_main.cpp test_gs_encoder.cpp)

include(GoogleTest)

target_link_libraries(test_gs_encoder PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_gs_encoder
         COMMAND test_gs_encoder)
