add_library(compressor_lib STATIC
  huffman_compressor.cpp
  file_bit_writer.cpp
)

target_include_directories(compressor_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(file-compressor main.cpp)

target_link_libraries(file-compressor PRIVATE compressor_lib)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(STRICT_FLAGS -Wall -Wextra -Wconversion -Wsign-conversion -Werror)
    
    target_compile_options(compressor_lib PRIVATE ${STRICT_FLAGS})
    target_compile_options(file-compressor PRIVATE ${STRICT_FLAGS})
endif()
