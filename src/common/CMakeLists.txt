include_directories(include)

add_subdirectory(src)

target_include_directories(common INTERFACE include)
