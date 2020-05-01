//
// Created by maxi on 15.04.20.
//

#include <cassert>
#include "Tree.h"

Node *Tree::getNextLeaf(Node *prevNode) {
    const std::lock_guard<std::mutex> lock(mutex);

    // Return nullptr if UNSAT
    if (dummyRoot.wasPruned)
        return nullptr;

    // Reset solverCounter
    while (prevNode->getLit() != 0) {
        prevNode->solverCounter--;
        prevNode = prevNode->parent;
    }

    // Look for a leaf on the path with the least solvers
    Node *nextNode = &dummyRoot;

    while (!nextNode->isLeaf) {

        // Child nodes exist because nextNode is not a leaf
        assert(!(nextNode->left->wasPruned && nextNode->right->wasPruned));

        if (nextNode->left->wasPruned) {
            nextNode = nextNode->right.get();
        } else if (nextNode->right->wasPruned) {
            nextNode = nextNode->left.get();
        } else {
            auto leftSolverCount = nextNode->left->solverCounter;
            auto rightSolverCount = nextNode->right->solverCounter;

            nextNode = leftSolverCount <= rightSolverCount ? nextNode->left.get() : nextNode->right.get();
        }
    }
    return nextNode;
}

void Tree::extend(Node *node, int lit) {
    const std::lock_guard<std::mutex> lock(mutex);
    if (node->isLeaf) {
        node->extend(lit);
    }
}

void Tree::prune(Node *node) {
    const std::lock_guard<std::mutex> lock(mutex);
    if (!node->wasPruned) {
        node->prune();
    }
}
