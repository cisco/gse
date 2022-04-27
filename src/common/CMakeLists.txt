add_library(gse_common STATIC
                       src/data_buffer_interface.cpp
                       src/octet_string.cpp)

if (WIN32)
    target_link_libraries(gse_common PRIVATE ws2_32)
endif()

target_compile_options(gse_common PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wpedantic -Wextra -Werror -Wall>
     $<$<CXX_COMPILER_ID:MSVC>: /WX>)

target_include_directories(gse_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
