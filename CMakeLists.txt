cmake_minimum_required(VERSION 3.17)

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
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_subdirectory(src)

include(CTest)

if(BUILD_TESTING AND NOT BUILD_SHARED_LIBS)
    add_subdirectory(test)
endif()
