
#include <fstream>
#include "Tree.h"
#include "SolverThread.h"

std::vector<std::unique_ptr<SolverThread>> solverThreads;
Tree tree;

void log(const std::string& filename) {
    std::ofstream logFile;
    logFile.open(filename);
    tree.log(logFile);
    logFile.close();
}

int main() {
    const char *PATH = "/home/maxi/gates/ecarev-110-1031-23-40-3.cnf";
    const size_t THREAD_COUNT = 2;

    for (int i = 0; i < THREAD_COUNT; i++) {
        solverThreads.emplace_back(new SolverThread(tree));
    }

    for (auto &solverThread : solverThreads) {
        solverThread->read(PATH);
        solverThread->start();
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
        solverThread->thread.join();
    }

    return 0;
}



