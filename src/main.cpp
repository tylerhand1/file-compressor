#include "huffman_compressor.h"
#include "memory_bit_reader.h"
#include "memory_bit_writer.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <io.h>
#define IS_TERMINAL _isatty(_fileno(stdin))
#else
#include <unistd.h>
#define IS_TERMINAL isatty(fileno(stdin))
#endif

std::vector<uint8_t> read_all_stdin() {
    std::vector<uint8_t> input_buffer;
    char ch;
    while (std::cin.get(ch)) {
        input_buffer.push_back(static_cast<uint8_t>(ch));
    }
    return input_buffer;
}

void write_all_stdout(const std::vector<uint8_t>& output_buffer) {
    std::cout.write(reinterpret_cast<const char*>(output_buffer.data()),
                    static_cast<std::streamsize>(output_buffer.size()));
    std::cout.flush();
}

int main(int argc, char* argv[]) {
    std::string mode = "-c";
    if (argc > 1) {
        mode = argv[1];
    }

    if (mode != "-c" && mode != "-d") {
        std::cerr << "Usage Error!\n";
        std::cerr << "To Compress:   " << argv[0] << " -c < input.txt > output.huff\n";
        std::cerr << "To Decompress: " << argv[0] << " -d < output.huff > restored.txt\n";
        return 1;
    }

    if (IS_TERMINAL) {
        std::cerr << "Error: No input data stream detected via pipe.\n";
        std::cerr << "Please use file redirection operators:\n";
        std::cerr << "  " << argv[0] << " " << mode << " < your_file.txt\n";
        return 1;
    }

    std::vector<uint8_t> input_bytes = read_all_stdin();
    if (input_bytes.empty())
        return 0;

    HuffmanCompressor huffmanProcessor;

    if (mode == "-c") {
        MemoryBitWriter writer;
        huffmanProcessor.compress(writer, input_bytes);

        const std::vector<uint8_t>& compressed = writer.get_data();

        double orig_size = static_cast<double>(input_bytes.size());
        double comp_size = static_cast<double>(compressed.size());
        double reduction = 100 - (comp_size / orig_size) * 100.0;

        std::cerr << "Original: " << input_bytes.size() << " bytes, "
                  << "Compressed: " << compressed.size() << " bytes "
                  << "(" << std::fixed << std::setprecision(2) << reduction
                  << "% reduction from original)\n";

        write_all_stdout(compressed);
    } else if (mode == "-d") {
        MemoryBitReader bitReader(input_bytes);
        std::vector<uint8_t> decompressed = huffmanProcessor.decompress(bitReader);
        write_all_stdout(decompressed);
    }

    return 0;
}
