#pragma once

#include <cstdint>
#include <vector>

class BitWriter {
  public:
    void write_bit(bool bit);
    void flush();
    const std::vector<uint8_t>& get_data() const {
        return buffer;
    }

  private:
    std::vector<uint8_t> buffer;
    uint8_t current_byte{0};
    int bit_count{0};
};
