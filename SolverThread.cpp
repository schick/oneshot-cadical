//
// Created by maxi on 30.04.20.
//

#include "SolverThread.h"
#include "cadical.hpp"

SolverThread::SolverThread(Tree &tree) : tree{tree} {};

std::atomic<bool> SolverThread::shouldTerminate{false};

void SolverThread::start() {
    thread = std::thread(&SolverThread::solve, this);
}

void SolverThread::solve() {
    currentLeaf = &tree.dummyRoot;

    while (!shouldTerminate.load()) {
        // Root was pruned
        if (currentLeaf == nullptr) {
            shouldTerminate.store(true);
            return;
        }

        // Assume literals depending on leaf
        assume(currentLeaf);

        // Try solving
        int result = solveLimited(10000);

        printf("Result: %d\n", result);

        if (result == 0) {
            currentLeaf->iterations++;
            if (currentLeaf->iterations.load() > 2 * currentLeaf->getDepth()) {
                int next = solver.next();
                int nextLit = next + 1;
                tree.extend(currentLeaf, nextLit);
                getNextLeaf();
            }

        } else if (result == 10) {
            shouldTerminate.store(true);
            return;

        } else if (result == 20) {
            tree.prune(currentLeaf);
            getNextLeaf();
        }
    }
}

void SolverThread::getNextLeaf() {
    currentLeaf = tree.getNextLeaf(currentLeaf);
}

void SolverThread::read(const char *path) {
    solver.read_dimacs(path, nVars, 0);
}

void SolverThread::assume(Node *node) {
    while (node->getLit() != 0) {
        solver.assume(node->getLit());
        node = node->getParent();
    }
}

int SolverThread::solveLimited(int conflits) {
    solver.limit("conflicts", conflits);
    return solver.solve();
}

