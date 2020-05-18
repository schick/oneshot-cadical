//
// Created by maxi on 30.04.20.
//

#ifndef ONESHOT_CADICAL_SOLVERTHREAD_H
#define ONESHOT_CADICAL_SOLVERTHREAD_H


#include "Tree.h"
#include "cadical.hpp"

#include <thread>
#include <functional>

class SolverThread {

public:
    SolverThread(Tree &tree, bool random, const std::string& extStrat, int limit);

    void start();

    std::thread thread;

    static std::atomic<bool> shouldTerminate;

    void read(const char *path);
    void assume(Node *node);
    int solveLimited(int conflicts);

private:
    CaDiCaL::Solver solver = CaDiCaL::Solver();

    void solve();
    std::function<int(int)> calcThreshhold;

    int limit;

    int nVars = 0;
    void getNextLeaf();

    Tree &tree;
    Node *currentLeaf = nullptr;
};


#endif //ONESHOT_CADICAL_SOLVERTHREAD_H
