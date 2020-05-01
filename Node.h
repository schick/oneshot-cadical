//
// Created by maxi on 30.04.20.
//

#ifndef ONESHOT_CADICAL_NODE_H
#define ONESHOT_CADICAL_NODE_H


#include <memory>
#include <atomic>

class Tree;

class Node {

    friend class Tree;

public:

    Node(Node *parent, int lit) : parent{parent}, lit{lit} {};

    int getLit() const {
        return lit;
    }

    Node *getParent() const {
        return parent;
    }

    std::atomic<int> iterations{0};

private:

    Node() = default;

    int lit = 0;
    Node *parent = nullptr;
    Node *neighbour = nullptr;

    // Need to use pointers because no forward declaration possible
    std::unique_ptr<Node> left = nullptr;
    std::unique_ptr<Node> right = nullptr;

    int solverCounter = 0;

    bool isLeaf = true;
    void extend(int newLit);

    bool wasPruned = false;
    void prune();
};


#endif //ONESHOT_CADICAL_NODE_H
