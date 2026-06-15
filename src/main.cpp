#include "huffman_compressor.h"

#include <cstdio>
#include <vector>

int main() {
    std::unique_ptr<Compressor> huffmanCompressor = std::make_unique<HuffmanCompressor>();

    std::vector<uint8_t> dummyData = {
        1,
        2,
        3,
        4,
    };

    std::vector<uint8_t> compressed_bytes = huffmanCompressor->compress(dummyData);

    std::fwrite(compressed_bytes.data(), sizeof(uint8_t), compressed_bytes.size(), stdout);

    std::fflush(stdout);
}
