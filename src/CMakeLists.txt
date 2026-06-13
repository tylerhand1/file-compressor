add_library(compressor_lib STATIC
  huffman_compressor.cpp
)

target_include_directories(compressor_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(file-compressor main.cpp)

target_link_libraries(file-compressor PRIVATE compressor_lib)
