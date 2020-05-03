//
// Created by maxi on 30.04.20.
//

#include "SolverThread.h"
#include "cadical.hpp"

SolverThread::SolverThread(const char *path, Tree &tree) : tree{tree}, path{path} {};

std::atomic<bool> SolverThread::shouldTerminate{false};

void SolverThread::start() {
    thread = std::thread(&SolverThread::solve, this);
}

void SolverThread::solve() {
    currentLeaf = &tree.dummyRoot;

    auto solver = CaDiCaL::Solver{};
    solver.read_dimacs(path, nVars, 0);

    while (!shouldTerminate.load()) {
        //Get next leaf
        currentLeaf = tree.getNextLeaf(currentLeaf);

        // Root was pruned
        if (currentLeaf == nullptr) {
            shouldTerminate.store(true);
            return;
        }

        // Assume literals depending on leaf
        assume(solver, currentLeaf);

        solver.limit("conflicts", 10000);
        int result = solver.solve();

        printf("Result: %d\n", result);

        if (result == 0) {
            currentLeaf->iterations++;
            if (currentLeaf->iterations.load() > 2 * currentLeaf->getDepth()) {
                int next = solver.next();
                int nextLit = next + 1;
                tree.extend(currentLeaf, nextLit);
            }

        } else if (result == 10) {
            shouldTerminate.store(true);
            return;

        } else if (result == 20) {
            tree.prune(currentLeaf);

        }
    }
}

void SolverThread::assume(CaDiCaL::Solver &solver, Node *node) {
    while (node->getLit() != 0) {
        solver.assume(node->getLit());
        node = node->getParent();
    }
}
