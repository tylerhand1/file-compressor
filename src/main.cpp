#include "bit_writer.h"
#include "file_bit_writer.h"
#include "huffman_compressor.h"

#include <cstdio>
#include <memory>
#include <vector>

int main() {
    std::unique_ptr<BitWriter> bitWriter = std::make_unique<FileBitWriter>();
    std::unique_ptr<Compressor> huffmanCompressor =
        std::make_unique<HuffmanCompressor>(std::move(bitWriter));

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
