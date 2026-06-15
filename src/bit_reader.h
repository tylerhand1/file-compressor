#pragma once
#include <cstdint>
#include <vector>

class BitReader {
  public:
    explicit BitReader(const std::vector<uint8_t>& source)
        : source(source), byte_idx(0), bit_idx(0) {
    }

    virtual ~BitReader() = default;

    bool read_bit() {
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

    uint8_t read_byte() {
        uint8_t value = 0;
        for (int i = 7; i >= 0; i--) {
            if (read_bit()) {
                value |= (1 << i);
            }
        }
        return value;
    }

    uint32_t read_uint32() {
        uint32_t value = 0;
        for (int i = 0; i < 4; ++i) {
            value = (value << 8) | read_byte();
        }
        return value;
    }

  private:
    const std::vector<uint8_t>& source;
    std::size_t byte_idx;
    int bit_idx;
};
