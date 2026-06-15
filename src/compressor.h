#pragma once
#include <vector>

class BitReader;

class Compressor {
  public:
    virtual ~Compressor() = default;

    virtual std::vector<uint8_t> compress(const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> decompress(BitReader& reader) = 0;
};
