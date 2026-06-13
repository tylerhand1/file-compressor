#include "huffman_compressor.hpp"

#include <iostream>

int main(int argc, char** argv) {
    std::unique_ptr<Compressor> huffmanCompressor = std::make_unique<HuffmanCompressor>();

    std::vector<uint8_t> dummyData = {1, 2, 3, 4};

    huffmanCompressor->compress(dummyData);
}
