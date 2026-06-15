#pragma once
#include "bit_reader.h"

#include <cstdint>
#include <vector>

class FileBitReader : public BitReader {
  public:
    explicit FileBitReader(const std::vector<uint8_t>& source) : BitReader(source) {
    }
};
