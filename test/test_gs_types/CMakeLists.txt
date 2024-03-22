add_executable(test_gs_types test_gs_types.cpp)

set_target_properties(test_gs_types
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_include_directories(test_gs_types PRIVATE ${libgse_SOURCE_DIR}/src)

target_link_libraries(test_gs_types PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_gs_types
         COMMAND test_gs_types)
