//
// Created by maxi on 15.04.20.
//

#ifndef ONESHOT_CADICAL_TREE_H
#define ONESHOT_CADICAL_TREE_H


#include <vector>
#include <mutex>
#include <memory>

struct Node {

    Node() = default;

    Node(Node *parent, int lit) : parent{parent}, lit{lit} {};

    int lit = 0;

    Node *parent = nullptr;

    std::unique_ptr<Node> left = nullptr;
    std::unique_ptr<Node> right = nullptr;

    // Use friend class to make this non public
    void extend(int newLit) {
        left = std::make_unique<Node>(this, newLit);
        right = std::make_unique<Node>(this, -newLit);
    }
};

class Tree {

public:

    explicit Tree(int firstLit) {
        dummyRoot.extend(firstLit);
        leaves = {dummyRoot.left.get(), dummyRoot.right.get()};
    }

    Node *getLeaf() {
        const std::lock_guard<std::mutex> lock(mutex);
        if (it == leaves.size()) {
            return nullptr;
        } else {
            return leaves.at(it++);
        }
    }

    void extend(Node *node, int lit) {
        const std::lock_guard<std::mutex> lock(mutex);
        node->extend(lit);
        leaves.push_back(node->left.get());
        leaves.push_back(node->right.get());
    }

private:

    size_t it = 0;
    Node dummyRoot;
    std::vector<Node *> leaves;

    std::mutex mutex;
};


#endif //ONESHOT_CADICAL_TREE_H
