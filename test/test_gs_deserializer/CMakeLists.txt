add_executable(test_gs_deserializer test_gs_deserializer.cpp)

set_target_properties(test_gs_deserializer
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_include_directories(test_gs_deserializer PRIVATE ${CMAKE_SOURCE_DIR}/src/gse/src)

target_link_libraries(test_gs_deserializer PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_gs_deserializer
         COMMAND test_gs_deserializer)
