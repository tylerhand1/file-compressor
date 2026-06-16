#include "huffman_compressor.h"
#include "memory_bit_reader.h"
#include "memory_bit_writer.h"

#include <catch2/catch_test_macros.hpp>

auto huffman_round_trip = [](const std::vector<uint8_t>& originalData) -> std::vector<uint8_t> {
    MemoryBitWriter writer;
    HuffmanCompressor compressor;
    compressor.compress(writer, originalData);

    auto compressed = writer.get_data();

    MemoryBitReader reader(compressed);
    MemoryBitWriter decompWriter;
    HuffmanCompressor decompressor;

    return decompressor.decompress(reader);
};

TEST_CASE("Initial Huffman round-trip passes", "[huffman]") {
    std::vector<uint8_t> originalData = {1, 2, 3, 4};
    REQUIRE(originalData == huffman_round_trip(originalData));
}

TEST_CASE("Empty data compression handles gracefully", "[huffman]") {
    std::vector<uint8_t> originalData = {};
    REQUIRE(originalData == huffman_round_trip(originalData));
}

TEST_CASE("Single character files encode smoothly", "[huffman]") {
    std::vector<uint8_t> originalData = {1};
    REQUIRE(originalData == huffman_round_trip(originalData));
}

TEST_CASE("Repeating single character frequencies handle edge case rules", "[huffman]") {
    std::vector<uint8_t> originalData = {1, 1, 1, 1, 1};
    REQUIRE(originalData == huffman_round_trip(originalData));
}

TEST_CASE("Full binary spectrum (0-255)", "[huffman]") {
    std::vector<uint8_t> completeBinarySpectrum;
    completeBinarySpectrum.reserve(256);
    for (int i = 0; i < 256; ++i) {
        completeBinarySpectrum.push_back(static_cast<uint8_t>(i));
    }

    REQUIRE(completeBinarySpectrum == huffman_round_trip(completeBinarySpectrum));
}
