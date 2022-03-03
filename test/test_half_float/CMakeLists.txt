add_executable(test_half_float test_main.cpp test_half_float.cpp)

target_include_directories(test_half_float PRIVATE ${CMAKE_SOURCE_DIR}/src/gse/src)

include(GoogleTest)

target_link_libraries(test_half_float PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_half_float
         COMMAND test_half_float)
