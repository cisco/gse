include_directories(${CMAKE_SOURCE_DIR}/src/gse/src)

add_executable(test_half_float test_main.cpp test_half_float.cpp)

include(GoogleTest)

target_link_libraries(test_half_float gse GTest::GTest GTest::Main)

add_test(NAME test_half_float
         COMMAND test_half_float)
