#pragma once

#include <cstdint>
#include <vector>

class BitWriter {
  public:
    virtual ~BitWriter() = default;
    virtual void write_bit(bool bit) = 0;
    virtual void flush() = 0;
    virtual const std::vector<uint8_t>& get_data() const = 0;
};
