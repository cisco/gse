cmake_minimum_required(VERSION 3.17)

# Building tests by default depends on whether this is a subproject
if (DEFINED PROJECT_NAME)
    # Option to control building tests
    option(gse_BUILD_TESTS "Build Tests for the Game State Encoder Library" OFF)
else()
    # Option to control building tests
    option(gse_BUILD_TESTS "Build Tests for the Game State Encoder Library" ON)
endif()

# Option to control library installation
option(gse_INSTALL "Install the Game State Encoder Library" ON)

project(libgse
        VERSION 1.0.0.0
        DESCRIPTION "Game State Encoder Library"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_subdirectory(src)

include(CTest)

if(BUILD_TESTING AND gse_BUILD_TESTS)
    add_subdirectory(test)
endif()
