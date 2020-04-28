
#include "cadical.hpp"

#include "Tree.h"

#include <atomic>
#include <thread>

std::atomic<int> res{0};
std::vector<std::thread> solverThreads;

void testNext() {
    auto *solver = new CaDiCaL::Solver;

    solver->add(-4);
    solver->add(0);

    int next = solver->next();
    printf("next: %d\n", next);
    int nextLit = next ^ 1 ? -((next >> 1) + 1) : ((next >> 1) + 1);
    printf("nextLit: %d\n", nextLit);

    delete solver;
}

void runCadicalSolver(const char *path, int nVars, Tree &tree) {
    auto *solver = new CaDiCaL::Solver;

    solver->read_dimacs(path, nVars, 1);

    while (res.load() != 10) {

        Node *leaf = nullptr;

        while (leaf == nullptr) {
            leaf = tree.getLeaf();
        }

        auto node = leaf;

        while (node->lit != 0) {
            solver->assume(node->lit);
            node = node->parent;
        }

        solver->limit("conflicts", 10000);
        int _res = solver->solve();

        printf("Result: %d\n", _res);

        if (_res == 10) {
            res.store(_res);
            return;
        } else if (_res == 0) {
            int next = solver->next();
            printf("next: %d\n", next);
            int nextLit = next + 1;
            printf("nextLit: %d\n", nextLit);

            tree.extend(leaf, nextLit);
        }
    }
    delete solver;
}

int main() {
    const char *path = "/home/maxi/gates/lei450-05b-05.cnf";
    int nVars = 2627;

    auto *solver = new CaDiCaL::Solver;
    solver->read_dimacs(path, nVars, 1);

    solver->limit("conflicts", 10000);
    int _res = solver->solve();

    printf("Result: %d\n", _res);

    if (_res == 10) {
        res.store(_res);
    } else if (_res == 0) {
        int next = solver->next();
        printf("next: %d\n", next);
        int nextLit = next + 1;
        printf("nextLit: %d\n", nextLit);
        delete solver;

        Tree tree{nextLit};

        for (int i = 0; i < 4; i++) {
            solverThreads.emplace_back(runCadicalSolver, path, nVars, std::ref(tree));
        }

        for (auto &thread : solverThreads) {
            thread.join();
        }

        return 0;
    }

}
