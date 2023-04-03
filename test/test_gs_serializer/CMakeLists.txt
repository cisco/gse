add_executable(test_gs_serializer test_gs_serializer.cpp)

set_target_properties(test_gs_serializer
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_link_libraries(test_gs_serializer PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_gs_serializer
         COMMAND test_gs_serializer)
