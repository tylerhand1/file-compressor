add_executable(unit_tests
  test_main.cpp
  test_huffman.cpp
)
target_link_libraries(unit_tests PRIVATE
  Catch2::Catch2WithMain
  compressor_lib
)

add_test(NAME CoreCompressionTests COMMAND unit_tests)
