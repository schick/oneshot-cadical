
#include <fstream>
#include "params.h"
#include "Tree.h"
#include "SolverThread.h"

std::vector<std::unique_ptr<SolverThread>> solverThreads;
Tree tree;

void log(const std::string &filename) {
    std::ofstream logFile;
    logFile.open(filename);
    tree.log(logFile);
    logFile.close();
}

int main(int argc, char *argv[]) {
    Parameters params;
    params.init(argc, argv);
    if (params.isSet("h") || params.isSet("help") || params.getFilename().empty()) {
        params.printUsage();
        return 0;
    } else {
        params.printParams();
    }

    for (int i = 0; i < params.getIntParam("threads"); i++) {
        solverThreads.emplace_back(
                new SolverThread(tree, params.isSet("random"), params.getParam("ext"), params.getIntParam("limit")));
    }

    for (auto &solverThread : solverThreads) {
        solverThread->read(params.getFilename().c_str());
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



