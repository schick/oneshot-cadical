
#include <fstream>
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

    int counter = 0;
    while (!SolverThread::shouldTerminate.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        std::ofstream logFile;
        logFile.open(std::to_string(counter++) + "log.gv");
        tree.log(logFile);
        logFile.close();
        printf("Wrote log\n");
    }

    for (auto &solverThread : solverThreads) {
        solverThread.thread.join();
    }

    return 0;
}

