include(FetchContent)

FetchContent_Declare(GTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.13.0
    OVERRIDE_FIND_PACKAGE)

if(WIN32)
    # For Windows: Prevent overriding the parent project's
    # compiler/linker settings
    # (https://github.com/google/googletest/tree/v1.13.0/googletest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# Prevent GTest from installing
set(INSTALL_GTEST OFF CACHE INTERNAL "Disable gtest installation")

FetchContent_MakeAvailable(GTest)

add_library(GTest::GTest INTERFACE IMPORTED GLOBAL)
add_library(GTest::Main INTERFACE IMPORTED GLOBAL)
target_link_libraries(GTest::GTest INTERFACE gtest_main)
