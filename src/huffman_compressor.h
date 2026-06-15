#pragma once
#include "bit_writer.h"
#include "compressor.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <queue>
#include <vector>

class HuffmanCompressor : public Compressor {
  public:
    explicit HuffmanCompressor(std::unique_ptr<BitWriter> writer) : writer(std::move(writer)) {
    }
    std::vector<uint8_t> compress(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> decompress(const std::vector<uint8_t>& data) override;

  private:
    std::unique_ptr<BitWriter> writer;

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

    using MinHeap = std::priority_queue<std::unique_ptr<HuffmanNode>,
                                        std::vector<std::unique_ptr<HuffmanNode>>, CompareNodes>;

    static void generate_path(const HuffmanCompressor::HuffmanNode* node,
                              std::vector<bool>& current_path,
                              std::unordered_map<uint8_t, std::vector<bool>>& lookup_table);

    void serialize_tree(const HuffmanCompressor::HuffmanNode* node);
};
