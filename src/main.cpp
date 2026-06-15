#include "huffman_compressor.h"

int main() {
    std::unique_ptr<Compressor> huffmanCompressor = std::make_unique<HuffmanCompressor>();

    std::vector<uint8_t> dummyData = {1, 2, 3, 4};

    huffmanCompressor->compress(dummyData);
}
