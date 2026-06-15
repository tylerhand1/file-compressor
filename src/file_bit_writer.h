#pragma once

#include "bit_writer.h"

#include <cstdint>
#include <vector>

class FileBitWriter : public BitWriter {
  public:
    void write_bit(bool bit) override;
    void flush() override;
    const std::vector<uint8_t>& get_data() const override {
        return buffer;
    }

  private:
    std::vector<uint8_t> buffer;
    uint8_t current_byte{0};
    int bit_count{0};
};
