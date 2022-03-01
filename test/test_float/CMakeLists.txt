add_executable(test_float test_main.cpp test_float.cpp)

include(GoogleTest)

target_link_libraries(test_float GTest::GTest GTest::Main)

add_test(NAME test_float
         COMMAND test_float)
