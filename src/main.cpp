// Copyright 2022 NNTU-CS
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include "tree.h"

static void printSeq(const std::vector<char>& seq) {
    for (char c : seq) std::cout << c;
    std::cout << '\n';
}

static std::vector<char> buildAlpha(int n) {
    std::vector<char> alpha;
    for (int i = 0; i < n; ++i)
        alpha.push_back(static_cast<char>('a' + i));
    return alpha;
}

static int randNum(int max) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, max);
    return dist(rng);
}

int main() {
    std::vector<char> src = { '1', '2', '3' };
    PMTree tree(src);

    auto all = getAllPerms(tree);
    std::cout << "getAllPerms {1,2,3}:\n";
    for (const auto& p : all) printSeq(p);
    std::cout << "total: " << all.size() << "\n\n";

    std::cout << "getPerm1 #1: "; printSeq(getPerm1(tree, 1));
    std::cout << "getPerm1 #3: "; printSeq(getPerm1(tree, 3));
    std::cout << "getPerm2 #2: "; printSeq(getPerm2(tree, 2));
    std::cout << "getPerm2 #6: "; printSeq(getPerm2(tree, 6));

    std::vector<char> inv = getPerm2(tree, 99);
    std::cout << "getPerm2 #99 (invalid): ";
    std::cout << (inv.empty() ? "(empty)" : "") << "\n\n";

    std::ofstream out("result/timing_data.txt");
    out << "n\tgetAllPerms(us)\tgetPerm1(us)\tgetPerm2(us)\n";

    using Clock = std::chrono::high_resolution_clock;
    using Us = std::chrono::microseconds;

    for (int n = 3; n <= 10; ++n) {
        auto alpha = buildAlpha(n);
        PMTree t(alpha);

        auto t0 = Clock::now();
        auto perms = getAllPerms(t);
        auto t1 = Clock::now();
        int64_t tAll = std::chrono::duration_cast<Us>(t1 - t0).count();

        int r1 = randNum(static_cast<int>(perms.size()));
        t0 = Clock::now();
        getPerm1(t, r1);
        t1 = Clock::now();
        int64_t tSlow = std::chrono::duration_cast<Us>(t1 - t0).count();

        int r2 = randNum(static_cast<int>(perms.size()));
        t0 = Clock::now();
        getPerm2(t, r2);
        t1 = Clock::now();
        int64_t tFast = std::chrono::duration_cast<Us>(t1 - t0).count();

        out << n << "\t" << tAll << "\t" << tSlow << "\t" << tFast << "\n";
        std::cout << "n=" << n
            << " tAll=" << tAll
            << "us tSlow=" << tSlow
            << "us tFast=" << tFast << "us\n";
    }

    out.close();
    return 0;
}
