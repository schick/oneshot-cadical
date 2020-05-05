//
// Created by maxi on 30.04.20.
//

#ifndef ONESHOT_CADICAL_NODE_H
#define ONESHOT_CADICAL_NODE_H


#include <memory>
#include <atomic>
#include <iostream>

class Tree;

class Node {

    friend class Tree;

public:

    Node(Node *parent, int lit) : parent{parent}, lit{lit}, depth{parent->depth + 1} {};

    Node() = default;
    int getLit();
    int getDepth();
    int getId();

    Node *getParent();

    void log(std::ostream &logFile);

    std::atomic<int> iterations{0};

protected:
    static std::atomic<int> s_id;

private:
    int id{++s_id};

    const int lit = 0;
    const int depth = 1;

    Node *parent = nullptr;
    Node *neighbour = nullptr;

    // Need to use pointers because no forward declaration possible
    std::unique_ptr<Node> left = nullptr;
    std::unique_ptr<Node> right = nullptr;

    int solverCounter = 0;
    int visits = 0;

    bool isLeaf = true;
    void extend(int newLit);

    bool wasPruned = false;
    void prune();
};


#endif //ONESHOT_CADICAL_NODE_H
