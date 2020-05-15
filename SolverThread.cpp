//
// Created by maxi on 30.04.20.
//

#include "SolverThread.h"

SolverThread::SolverThread(Tree &tree) : tree{tree} {};

std::atomic<bool> SolverThread::shouldTerminate{false};

void SolverThread::start() {
    thread = std::thread(&SolverThread::solve, this);
}

void SolverThread::learn_unit(int lit) {
    learntClauses.emplace(lit);
}

void SolverThread::learn_clause(std::vector<int> &clause, int glue) {
    learntClauses.emplace(clause);
}

void SolverThread::solve() {
    currentLeaf = &tree.dummyRoot;

    std::function<void(int)> learn_unit_callback = std::bind(&SolverThread::learn_unit, this, std::placeholders::_1);
    solver.add_learned_unit_lit_callback(learn_unit_callback);

    std::function<void(std::vector<int>&, int)> learn_clause_callback = std::bind(&SolverThread::learn_clause, this, std::placeholders::_1, std::placeholders::_2);
    solver.add_learned_clause_callback(learn_clause_callback);

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
        printf("Learned: %d\n", learntClauses.size());

        if (result == 0) {
            currentLeaf->iterations++;
            if (currentLeaf->iterations.load() > 2 * currentLeaf->getDepth()) {
                int nextLit = solver.next();
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



