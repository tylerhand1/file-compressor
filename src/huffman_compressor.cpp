#include "huffman_compressor.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

void HuffmanCompressor::generate_path(
    const HuffmanCompressor::HuffmanNode* node, std::vector<bool>& current_path,
    std::unordered_map<uint8_t, std::vector<bool>>& lookup_table) {
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

void HuffmanCompressor::serialize_tree(const HuffmanCompressor::HuffmanNode* node) {
    if (!node)
        return;

    if (node->character.has_value()) {
        writer->write_bit(true);

        uint8_t ch = node->character.value();
        for (int i = 7; i >= 0; --i) {
            writer->write_bit(((ch >> i) & 1) == 1);
        }
        return;
    }

    writer->write_bit(false);

    serialize_tree(node->left.get());
    serialize_tree(node->right.get());
}

std::vector<uint8_t> HuffmanCompressor::compress(const std::vector<uint8_t>& data) {
    if (data.empty()) {
        return {};
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

            min_heap.push(std::move(leaf));
        }
    }

    while (min_heap.size() > 1) {
        auto left_child = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(min_heap.top()));
        min_heap.pop();

        auto right_child = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(min_heap.top()));
        min_heap.pop();

        auto parent = std::make_unique<HuffmanNode>();
        parent->frequency = left_child->frequency + right_child->frequency;
        parent->left = std::move(left_child);
        parent->right = std::move(right_child);

        min_heap.push(std::move(parent));
    }

    std::unique_ptr<HuffmanNode> tree_root = nullptr;
    if (!min_heap.empty()) {
        tree_root = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(min_heap.top()));
        min_heap.pop();
    }

    std::unordered_map<uint8_t, std::vector<bool>> lookup_table;
    if (tree_root) {
        std::vector<bool> buffer;
        generate_path(tree_root.get(), buffer, lookup_table);
    }

    std::array<std::vector<bool>, 256> fast_lookup;
    for (const auto& [byte_val, bit_vector] : lookup_table) {
        fast_lookup[byte_val] = bit_vector;
    }

    if (tree_root) {
        serialize_tree(tree_root.get());
    }

    for (uint8_t byte : data) {
        const std::vector<bool>& bit_vector = fast_lookup[byte];

        for (bool bit : bit_vector) {
            writer->write_bit(bit);
        }
    }

    writer->flush();

    return writer->get_data();
}

std::vector<uint8_t> HuffmanCompressor::decompress(const std::vector<uint8_t>& data) {
    std::cout << "Expanding" << std::endl;
    return data;
}
