#include "file_bit_writer.h"

#include <cstdint>

void FileBitWriter::write_bit(bool bit) {
    current_byte = static_cast<uint8_t>(current_byte << 1);

    if (bit)
        current_byte |= 1;

    bit_count++;

    if (bit_count == 8) {
        buffer.push_back(current_byte);
        current_byte = 0;
        bit_count = 0;
    }
}

void FileBitWriter::flush() {
    if (bit_count > 0) {
        current_byte = static_cast<uint8_t>(current_byte << (8 - bit_count));
        buffer.push_back(current_byte);
        current_byte = 0;
        bit_count = 0;
    }
}
