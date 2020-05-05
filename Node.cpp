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

std::atomic<int> Node::s_id(0);

Node *Node::getParent() {
    return parent;
}

int Node::getId() {
    return id;
}

void Node::log(std::ostream &logFile) {
    logFile << getId()  << " [label=\"" << lit << "|" << solverCounter << "|" << visits << "|" << iterations << "|" << isLeaf << "|" << wasPruned << "|" << depth << "\" ];\n";
    if (!isLeaf) {
        logFile << getId() << " -> " << left->getId() << "\n";
        left->log(logFile);
        logFile << getId() << " -> " << right->getId() << "\n";
        right->log(logFile);
    }
}


