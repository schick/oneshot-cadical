
#include "Tree.h"
#include "SolverThread.h"

static std::vector<SolverThread> solverThreads;

int main() {
    const char *path = "/home/maxi/gates/ecarev-110-1031-23-40-3.cnf";

    Tree tree{};

    for (int i = 0; i < 2; i++) {
        solverThreads.emplace_back(path, tree);
    }

    for (auto &solverThread : solverThreads) {
        solverThread.start();
    }

    for (auto &solverThread : solverThreads) {
        solverThread.thread.join();
    }

    return 0;
}

