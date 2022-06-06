add_executable(test_gs_deserializer test_gs_deserializer.cpp)

target_include_directories(test_gs_deserializer PRIVATE ${CMAKE_SOURCE_DIR}/src/gse/src)

include(GoogleTest)

target_link_libraries(test_gs_deserializer PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_gs_deserializer
         COMMAND test_gs_deserializer)
