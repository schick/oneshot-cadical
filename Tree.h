//
// Created by maxi on 15.04.20.
//

#ifndef ONESHOT_CADICAL_TREE_H
#define ONESHOT_CADICAL_TREE_H


#include <mutex>
#include "Node.h"

class Tree {

public:

    Tree() = default;

    Node dummyRoot;

    Node *getNextLeaf(Node *prevNode);

    void extend(Node *node, int lit);

    void prune(Node *node);

private:
    std::mutex mutex;
};


#endif //ONESHOT_CADICAL_TREE_H
