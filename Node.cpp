//
// Created by maxi on 30.04.20.
//

#include "Node.h"

void Node::prune() {
    wasPruned = true;
    // Workaround for root node
    if (lit != 0)
        if (neighbour->wasPruned)
            parent->prune();
}

void Node::extend(int newLit) {
    left = std::make_unique<Node>(this, newLit);
    right = std::make_unique<Node>(this, -newLit);
    // Set neighbours
    left->neighbour = right.get();
    right->neighbour = left.get();
    // Flag node
    isLeaf = false;
}

int Node::getLit() {
    return lit;
}

int Node::getDepth() {
    return depth;
}

Node *Node::getParent() {
    return parent;
}

void Node::log(std::ostream &logFile) {
    logFile << lit  << " [label=\"" << lit << "|" << solverCounter << "|" << iterations << "|" << isLeaf << "|" << wasPruned << "|" << depth << "\" ];\n";
    if (!isLeaf) {
        logFile << lit << " -> " << left->lit << "\n";
        left->log(logFile);
        logFile << lit << " -> " << right->lit << "\n";
        right->log(logFile);
    }
}


