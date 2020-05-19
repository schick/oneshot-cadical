
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

struct counting_iterator {
    size_t count;
    counting_iterator& operator++() { ++count; return *this; }

    struct black_hole { void operator=(Clause) {} };
    black_hole operator*() { return black_hole(); }

    // other iterator stuff may be needed
};



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
    std::this_thread::sleep_for(std::chrono::milliseconds(300000));
    SolverThread::shouldTerminate.store(true);

    for (auto &solverThread : solverThreads) {
        solverThread->thread.join();
    }

    for (auto &solverThread : solverThreads) {
        solverThread->learn();
    }

    for (size_t i = 0; i < solverThreads.size(); i++) {
        auto learnt1 = solverThreads.at(i)->learntClauses;
        printf("For Solver with id: %zu\n", i);
        for (size_t j = i + 1; j < solverThreads.size(); j++) {
            auto learnt2 = solverThreads.at(j)->learntClauses;
            size_t count = set_intersection(
                    learnt1.begin(), learnt1.end(), learnt2.begin(), learnt2.end(), counting_iterator()).count;
            double interceptage = 1 - (double) count / learnt1.size();
            printf("Interceptage with Solver %zu: %f\n", j, interceptage);
        }
    }


    return 0;
}



