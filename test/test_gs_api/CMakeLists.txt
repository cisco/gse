add_executable(test_gs_api test_gs_api.cpp)

set_target_properties(test_gs_api
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

include(GoogleTest)

target_link_libraries(test_gs_api PRIVATE gse GTest::GTest GTest::Main)

add_test(NAME test_gs_api
         COMMAND test_gs_api)
