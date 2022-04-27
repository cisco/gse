add_library(gse
            src/gs_api.cpp
            src/gs_api_internal.cpp
            src/gs_decoder.cpp
            src/gs_encoder.cpp
            src/gs_deserializer.cpp
            src/gs_serializer.cpp
            src/half_float.cpp)

set_target_properties(gse_common PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})

target_link_libraries(gse PRIVATE gse_common)

target_compile_options(gse PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wpedantic -Wextra -Werror -Wall>
     $<$<CXX_COMPILER_ID:MSVC>: /WX>)

target_include_directories(gse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
