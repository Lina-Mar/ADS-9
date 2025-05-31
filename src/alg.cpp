// Copyright 2022 NNTU-CS
#include  <iostream>
#include  <fstream>
#include  <locale>
#include  <cstdlib>
#include <memory>
#include <vector>
#include <algorithm>
#include  "tree.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::move;

PMTree::PMTree(const vector<char>& elements) {
    if (elements.empty()) {
        root = nullptr;
        total_permutations = 0;
        return;
    }
    root = make_unique<Node>('\0');
    total_permutations = 1;
    for (size_t i = 2; i <= elements.size(); ++i) {
        total_permutations *= i;
    }
    buildTree(root.get(), elements);
}

void PMTree::buildTree(Node* parent, const vector<char>& remaining) {
    if (remaining.empty()) return;
    for (char elem : remaining) {
        auto child = make_unique<Node>(elem);
        vector<char> new_remaining;
        new_remaining.reserve(remaining.size() - 1);
        for (char e : remaining) {
            if (e != elem) {
                new_remaining.push_back(e);
            }
        }
        buildTree(child.get(), new_remaining);
        parent->children.push_back(move(child));
    }
}

void collectPerms(const PMTree::Node* node, vector<char>& current,
                 vector<vector<char>>& result) {
    if (!node) return;
    if (node->value != '\0') {
        current.push_back(node->value);
    }
    if (node->children.empty()) {
        result.push_back(current);
    } else {
        for (const auto& child : node->children) {
            collectPerms(child.get(), current, result);
        }
    }
    if (node->value != '\0') {
        current.pop_back();
    }
}

vector<vector<char>> getAllPerms(const PMTree& tree) {
    vector<vector<char>> result;
    if (!tree.getRoot()) return result;
    vector<char> current;
    collectPerms(tree.getRoot(), current, result);
    return result;
}

vector<char> getPerm1(const PMTree& tree, int num) {
    if (num < 1 || static_cast<size_t>(num) > tree.getTotalPermutations()) {
        return {};
    }
    auto all_perms = getAllPerms(tree);
    return all_perms[num - 1];
}

vector<char> getPerm2(const PMTree& tree, int num) {
    if (num < 1 || static_cast<size_t>(num) > tree.getTotalPermutations()) {
        return {};
    }
    vector<char> result;
    const PMTree::Node* current = tree.getRoot();
    int remaining_num = num - 1;
    while (!current->children.empty()) {
        int fact = 1;
        int children_count = current->children.size();
        for (int i = 2; i <= children_count - 1; ++i) {
            fact *= i;
        }
        if (fact == 0) fact = 1;
        int branch_index = remaining_num / fact;
        remaining_num %= fact;
        current = current->children[branch_index].get();
        result.push_back(current->value);
    }
    return result;
}
