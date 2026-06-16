#include "huffman_compressor.h"

#include "bit_reader.h"
#include "bit_writer.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

void HuffmanCompressor::compress(BitWriter& writer, const std::vector<uint8_t>& data) {
    if (data.empty()) {
        return;
    }

    std::array<std::size_t, 256> frequencies{};
    HuffmanCompressor::MinHeap min_heap;

    for (uint8_t byte : data) {
        frequencies[byte]++;
    }

    for (std::size_t i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            auto leaf = std::make_unique<HuffmanNode>();
            leaf->frequency = static_cast<int>(frequencies[i]);
            leaf->character = static_cast<std::uint8_t>(i);

            min_heap.push_back(std::move(leaf));
        }
    }

    auto comp = CompareNodes();
    std::make_heap(min_heap.begin(), min_heap.end(), comp);

    while (min_heap.size() > 1) {
        std::pop_heap(min_heap.begin(), min_heap.end(), comp);
        auto left_child = std::move(min_heap.back());
        min_heap.pop_back();

        std::pop_heap(min_heap.begin(), min_heap.end(), comp);
        auto right_child = std::move(min_heap.back());
        min_heap.pop_back();

        auto parent = std::make_unique<HuffmanNode>();
        parent->frequency = left_child->frequency + right_child->frequency;
        parent->left = std::move(left_child);
        parent->right = std::move(right_child);

        min_heap.push_back(std::move(parent));
        std::push_heap(min_heap.begin(), min_heap.end(), comp);
    }

    std::unique_ptr<HuffmanNode> tree_root = nullptr;
    if (!min_heap.empty()) {
        std::pop_heap(min_heap.begin(), min_heap.end(), comp);
        tree_root = std::move(min_heap.back());
        min_heap.pop_back();
    }

    std::array<std::vector<bool>, 256> lookup_table;
    if (tree_root) {
        std::vector<bool> buffer;
        generate_path(tree_root.get(), buffer, lookup_table);
    }

    uint32_t total_size = static_cast<uint32_t>(data.size());
    for (int i = 3; i >= 0; --i) {
        uint8_t byte = (total_size >> (i * 8)) & 0xFF;
        for (int b = 7; b >= 0; --b) {
            writer.write_bit(((byte >> b) & 1) == 1);
        }
    }

    if (tree_root) {
        serialize_tree(writer, tree_root.get());
    }

    for (uint8_t byte : data) {
        const std::vector<bool>& bit_vector = lookup_table[byte];

        for (bool bit : bit_vector) {
            writer.write_bit(bit);
        }
    }

    writer.flush();
}

std::vector<uint8_t> HuffmanCompressor::decompress(BitReader& reader) {
    std::vector<uint8_t> decompressed_data;

    uint32_t total_bytes = reader.read_uint32();
    if (total_bytes == 0) {
        return {};
    }

    std::unique_ptr<HuffmanNode> root = deserialize_tree(reader);

    if (!root->left && !root->right) {
        return std::vector<uint8_t>(total_bytes, root->character.value());
    }

    for (uint32_t i = 0; i < total_bytes; i++) {
        const HuffmanNode* current = root.get();

        while (!current->character.has_value()) {
            bool bit = reader.read_bit();
            if (bit) {
                current = current->right ? current->right.get() : current->left.get();
            } else {
                current = current->left ? current->left.get() : current->right.get();
            }
        }

        decompressed_data.push_back(current->character.value());
    }

    return decompressed_data;
}

void HuffmanCompressor::generate_path(const HuffmanCompressor::HuffmanNode* node,
                                      std::vector<bool>& current_path,
                                      std::array<std::vector<bool>, 256>& lookup_table) {
    if (!node)
        return;

    if (node->character.has_value()) {
        lookup_table[node->character.value()] = current_path;
        return;
    }

    current_path.push_back(false);
    generate_path(node->left.get(), current_path, lookup_table);
    current_path.pop_back();

    current_path.push_back(true);
    generate_path(node->right.get(), current_path, lookup_table);
    current_path.pop_back();
}

void HuffmanCompressor::serialize_tree(BitWriter& writer,
                                       const HuffmanCompressor::HuffmanNode* node) {
    if (!node)
        return;

    if (node->character.has_value()) {
        writer.write_bit(true);

        uint8_t ch = node->character.value();
        for (int i = 7; i >= 0; --i) {
            writer.write_bit(((ch >> i) & 1) == 1);
        }
        return;
    }

    writer.write_bit(false);

    if (node->left) {
        serialize_tree(writer, node->left.get());
    }
    if (node->right) {
        serialize_tree(writer, node->right.get());
    }
}

std::unique_ptr<HuffmanCompressor::HuffmanNode>
HuffmanCompressor::deserialize_tree(BitReader& reader) {
    auto node = std::make_unique<HuffmanCompressor::HuffmanNode>();

    if (reader.read_bit()) {
        node->character = reader.read_byte();
        node->left = nullptr;
        node->right = nullptr;
    } else {
        node->character = std::nullopt;
        node->left = deserialize_tree(reader);
        node->right = deserialize_tree(reader);
    }

    return node;
}
