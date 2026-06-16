#pragma once
#include "bit_reader.h"

#include <cstdint>
#include <vector>

class MemoryBitReader : public BitReader {
  public:
    explicit MemoryBitReader(const std::vector<uint8_t>& source_data)
        : source(source_data), byte_idx(0), bit_idx(0) {
    }

    bool read_bit() override {
        if (byte_idx >= source.size()) {
            return false;
        }

        bool bit = (source[byte_idx] >> (7 - bit_idx)) & 1;
        bit_idx++;
        if (bit_idx == 8) {
            bit_idx = 0;
            byte_idx++;
        }
        return bit;
    }

  private:
    const std::vector<uint8_t> source;
    std::size_t byte_idx;
    int bit_idx;
};
