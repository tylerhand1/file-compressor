#pragma once
#include "compressor.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

class HuffmanCompressor : public Compressor {
  public:
    std::vector<uint8_t> compress(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> decompress(const std::vector<uint8_t>& data) override;

  private:
    // TODO: setup tree for Huffman
};
