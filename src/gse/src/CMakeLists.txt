add_library(gse gs_api.cpp
                gs_api_internal.cpp
                gs_decoder.cpp
                gs_encoder.cpp
                gs_deserializer.cpp
                gs_serializer.cpp)

set_target_properties(common PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
target_link_libraries(gse PRIVATE common)

target_compile_options(gse PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wpedantic -Wextra -Werror -Wall>
     $<$<CXX_COMPILER_ID:MSVC>: /WX>)

target_include_directories(gse PUBLIC PUBLIC ${CMAKE_SOURCE_DIR}/src/gse/include)
