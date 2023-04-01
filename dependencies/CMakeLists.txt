if(gse_BUILD_TESTS)
    find_package(GTest)

    if(NOT GTest_FOUND)
        message(STATUS "Fetching GTest since it is not available on the system")
        add_subdirectory(googletest)
    endif()
endif()
