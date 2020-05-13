
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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Missing input file\n");
        return 1;
    }
    const char *PATH = argv[1];
    const auto THREAD_COUNT = std::thread::hardware_concurrency();
    printf("Using %d Threads\n", THREAD_COUNT);

    for (int i = 0; i < THREAD_COUNT; i++) {
        solverThreads.emplace_back(new SolverThread(tree));
    }

    for (auto &solverThread : solverThreads) {
        solverThread->read(PATH);
        solverThread->start();
    }

#ifdef LOG
    int counter = 0;
    while (!SolverThread::shouldTerminate.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        log(std::to_string(counter++) + "log.gv");
        printf("Wrote log\n");
    }

    log("result.gv");
    printf("Wrote result\n");
#endif

    for (auto &solverThread : solverThreads) {
        solverThread->thread.join();
    }

    return 0;
}



