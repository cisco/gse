include_directories(${CMAKE_SOURCE_DIR}/src/gse/src)

add_executable(test_gs_deserializer test_main.cpp test_gs_deserializer.cpp)

include(GoogleTest)

target_link_libraries(test_gs_deserializer gse common GTest::GTest GTest::Main)

add_test(NAME test_gs_deserializer
         COMMAND test_gs_deserializer)
