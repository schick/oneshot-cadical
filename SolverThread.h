//
// Created by maxi on 30.04.20.
//

#ifndef ONESHOT_CADICAL_SOLVERTHREAD_H
#define ONESHOT_CADICAL_SOLVERTHREAD_H


#include "Tree.h"
#include "cadical.hpp"

#include <thread>

class SolverThread {

public:

    SolverThread(const char *path, Tree &tree);

    void start();

    std::thread thread;

    static std::atomic<bool> shouldTerminate;

private:

    void solve();
    const char *path;

    int nVars = 0;

    inline void getNextLeaf();

    static inline void assume(CaDiCaL::Solver &solver, Node *node);

    Tree &tree;
    Node *currentLeaf = nullptr;

};


#endif //ONESHOT_CADICAL_SOLVERTHREAD_H
