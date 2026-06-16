#pragma once
#include "bit_reader.h"
#include "bit_writer.h"
#include "compressor.h"

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

class HuffmanCompressor : public Compressor {
  public:
    void compress(BitWriter& writer, const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> decompress(BitReader& reader) override;

  private:
    struct HuffmanNode {
        int frequency{0};

        std::optional<uint8_t> character;

        std::unique_ptr<HuffmanNode> left;
        std::unique_ptr<HuffmanNode> right;
    };

    struct CompareNodes {
        bool operator()(const std::unique_ptr<HuffmanNode>& a,
                        const std::unique_ptr<HuffmanNode>& b) const {
            return a->frequency > b->frequency;
        }
    };

    using MinHeap = std::vector<std::unique_ptr<HuffmanNode>>;

    static void generate_path(const HuffmanCompressor::HuffmanNode* node,
                              std::vector<bool>& current_path,
                              std::array<std::vector<bool>, 256>& lookup_table);

    void serialize_tree(BitWriter& writer, const HuffmanCompressor::HuffmanNode* node);

    std::unique_ptr<HuffmanCompressor::HuffmanNode> deserialize_tree(BitReader& reader);
};
