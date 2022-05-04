add_library(gse
            data_buffer_interface.cpp
            gs_api.cpp
            gs_api_internal.cpp
            gs_decoder.cpp
            gs_deserializer.cpp
            gs_encoder.cpp
            gs_serializer.cpp
            half_float.cpp
            octet_string.cpp)

target_compile_options(gse PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wpedantic -Wextra -Werror -Wall>
     $<$<CXX_COMPILER_ID:MSVC>: /WX>)

# Specify the internal and public include directories
target_include_directories(gse
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/gse/include>
)
