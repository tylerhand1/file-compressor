#pragma once
#include "bit_writer.h"

#include <vector>

class BitReader;

class Compressor {
  public:
    virtual ~Compressor() = default;

    virtual void compress(BitWriter& writer, const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> decompress(BitReader& reader) = 0;
};
