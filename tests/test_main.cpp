#include <catch2/catch_test_macros.hpp>

int mock_compress_file(int input_size) {
    return input_size / 2;
}

TEST_CASE("Compression "
          "ratio "
          "calculations "
          "are correct",
          "[compression"
          "]") {
    REQUIRE(mock_compress_file(100) == 50);
    REQUIRE(mock_compress_file(0) == 0);
}
