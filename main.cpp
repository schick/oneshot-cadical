
#include <fstream>
#include "Tree.h"
#include "SolverThread.h"

std::vector<SolverThread> solverThreads;
Tree tree;

void log(const std::string& filename) {
    std::ofstream logFile;
    logFile.open(filename);
    tree.log(logFile);
    logFile.close();
}

int main() {
    const char *path = "/home/maxi/gates/ecarev-110-1031-23-40-3.cnf";

    for (int i = 0; i < 2; i++) {
        solverThreads.emplace_back(path, tree);
    }

    for (auto &solverThread : solverThreads) {
        solverThread.start();
    }

    int counter = 0;
    while (!SolverThread::shouldTerminate.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        log(std::to_string(counter++) + "log.gv");
        printf("Wrote log\n");
    }

    log("result.gv");
    printf("Wrote result\n");

    for (auto &solverThread : solverThreads) {
        solverThread.thread.join();
    }

    return 0;
}



