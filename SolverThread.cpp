//
// Created by maxi on 30.04.20.
//

#include "SolverThread.h"
#include "cadical.hpp"
#include <cmath>
#include <stdexcept>

SolverThread::SolverThread(Tree &tree, bool random, const std::string& extStrat, int limit) : tree{tree}, limit{limit} {
    if (random)
        solver.set("seed", std::rand());

    if (extStrat == "lin")
        calcThreshhold = [](int depth) { return depth; };
    else if (extStrat == "quad")
        calcThreshhold = [](int depth){return std::pow(depth, 2);};
    else if (extStrat == "expo")
        calcThreshhold = [](int depth){return std::pow(2, depth);};
    else
        throw std::invalid_argument("Unknown option for extension strategy.");
}

std::atomic<bool> SolverThread::shouldTerminate{false};

void SolverThread::start() {
    thread = std::thread(&SolverThread::solve, this);
}

void SolverThread::solve() {
    currentLeaf = &tree.dummyRoot;

    while (!shouldTerminate.load()) {
        // Root was pruned
        if (currentLeaf == nullptr) {
            printf("UNSATISFIABLE\n");
            shouldTerminate.store(true);
            return;
        }

        // Assume literals depending on leaf
        assume(currentLeaf);

        // Try solving
        int result = solveLimited(limit);


        if (result == 0) {
            currentLeaf->iterations++;
            if (currentLeaf->iterations.load() > calcThreshhold(currentLeaf->getDepth())) {
                int next = solver.next();
                int nextLit = next + 1;
                tree.extend(currentLeaf, nextLit);
                getNextLeaf();
            }

        } else if (result == 10) {
            printf("SATISFIABLE\n");
            shouldTerminate.store(true);
            return;

        } else if (result == 20) {
            printf("NODE PRUNED\n");
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



