#include "huffman_compressor.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Initial Huffman round-trip passes", "[huffman]") {
    std::unique_ptr<BitWriter> bitWriter = std::make_unique<FileBitWriter>();
    std::unique_ptr<Compressor> compressor =
        std::make_unique<HuffmanCompressor>(std::move(bitWriter));

    std::vector<uint8_t> originalData = {1, 2, 3, 4};

    auto compressed = compressor->compress(originalData);
    auto decompressed = compressor->decompress(compressed);

    REQUIRE(originalData == decompressed);
}
