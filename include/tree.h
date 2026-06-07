// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <memory>
#include <vector>

struct PMNode {
	char val;
	std::vector<std::unique_ptr<PMNode>> kids;
	explicit PMNode(char c) : val(c) {}
};

class PMTree {
public:
	explicit PMTree(const std::vector<char>& chars);
	const PMNode* getRoot() const { return root_.get(); }
	int depth() const { return depth_; }

private:
	std::unique_ptr<PMNode> root_;
	int depth_;
	static void expand(PMNode* node, std::vector<char> left);
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(const PMTree& tree, int num);
std::vector<char> getPerm2(const PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
