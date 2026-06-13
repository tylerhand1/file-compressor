add_executable(unit_tests
  test_main.cpp
)

target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

add_test(NAME CoreCompressionTests COMMAND unit_tests)

target_include_directories(file-compressor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
