add_library(gse
            data_buffer.cpp
            gs_api.cpp
            gs_api_internal.cpp
            gs_decoder.cpp
            gs_deserializer.cpp
            gs_encoder.cpp
            gs_serializer.cpp
            half_float.cpp
            octet_string.cpp)

set_target_properties(gse
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
target_compile_options(gse PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wpedantic -Wextra -Wall>
     $<$<CXX_COMPILER_ID:MSVC>: >)

if(WIN32)
    target_link_libraries(gse PRIVATE ws2_32)
endif()

# Specify the internal and public include directories
target_include_directories(gse
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

# Install target and associated include files
if(gse_INSTALL)
    install(TARGETS gse EXPORT gseTargets ARCHIVE)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ TYPE INCLUDE)
    install(EXPORT gseTargets
            FILE gseTargets.cmake
            NAMESPACE GSE::
            DESTINATION lib/cmake/gse/)
endif()
