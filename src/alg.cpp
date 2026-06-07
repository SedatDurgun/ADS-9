// Copyright 2022 NNTU-CS
#include <algorithm>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>
#include "tree.h"

void PMTree::expand(PMNode* node, std::vector<char> left) {
    if (left.empty()) return;
    std::sort(left.begin(), left.end());
    for (int i = 0; i < static_cast<int>(left.size()); ++i) {
        auto child = std::make_unique<PMNode>(left[i]);
        std::vector<char> rest;
        for (int j = 0; j < static_cast<int>(left.size()); ++j) {
            if (j != i) rest.push_back(left[j]);
        }
        expand(child.get(), std::move(rest));
        node->kids.push_back(std::move(child));
    }
}

PMTree::PMTree(const std::vector<char>& chars)
    : root_(std::make_unique<PMNode>('\0')),
    depth_(static_cast<int>(chars.size())) {
    std::vector<char> sorted = chars;
    std::sort(sorted.begin(), sorted.end());
    expand(root_.get(), std::move(sorted));
}

namespace {

    void collect(const PMNode* node,
        std::vector<char>* path,
        std::vector<std::vector<char>>* out) {
        if (node->kids.empty()) {
            out->push_back(*path);
            return;
        }
        for (const auto& k : node->kids) {
            path->push_back(k->val);
            collect(k.get(), path, out);
            path->pop_back();
        }
    }

    int64_t fact(int n) {
        int64_t r = 1;
        for (int i = 2; i <= n; ++i) r *= i;
        return r;
    }

} 

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> out;
    std::vector<char> path;
    collect(tree.getRoot(), &path, &out);
    return out;
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
    if (num <= 0) return {};
    auto all = getAllPerms(tree);
    if (num > static_cast<int>(all.size())) return {};
    return all[num - 1];
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
    if (num <= 0) return {};
    int n = tree.depth();
    if (static_cast<int64_t>(num) > fact(n)) return {};
    std::vector<char> out;
    out.reserve(n);
    int64_t rem = static_cast<int64_t>(num) - 1;
    const PMNode* cur = tree.getRoot();
    for (int d = n; d > 0; --d) {
        int64_t block = fact(d - 1);
        int pick = static_cast<int>(rem / block);
        rem %= block;
        cur = cur->kids[pick].get();
        out.push_back(cur->val);
    }
    return out;
}
