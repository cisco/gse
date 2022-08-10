cmake_minimum_required(VERSION 3.17)

# Building tests by default depends on whether this is a subproject
if (DEFINED PROJECT_NAME)
    option(gse_BUILD_TESTS "Build Tests for the Game State Encoder Library" OFF)
else()
    option(gse_BUILD_TESTS "Build Tests for the Game State Encoder Library" ON)
endif()

# Option to control library installation
option(gse_INSTALL "Install the Game State Encoder Library" ON)

project(libgse
        VERSION 1.0.0.0
        DESCRIPTION "Game State Encoder Library"
        LANGUAGES CXX)

add_subdirectory(src)

include(CTest)

if(BUILD_TESTING AND gse_BUILD_TESTS)
    add_subdirectory(test)
endif()
