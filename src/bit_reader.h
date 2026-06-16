#pragma once
#include <cstdint>

class BitReader {
  public:
    virtual ~BitReader() = default;

    virtual bool read_bit() = 0;

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
};
