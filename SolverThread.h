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

    explicit SolverThread(Tree &tree);

    void start();

    std::thread thread;

    static std::atomic<bool> shouldTerminate;

    void read(const char *path);
    void assume(Node *node);
    int solveLimited(int conflits);

private:
    CaDiCaL::Solver solver = CaDiCaL::Solver();

    void solve();

    int nVars = 0;
    void getNextLeaf();

    Tree &tree;
    Node *currentLeaf = nullptr;

};


#endif //ONESHOT_CADICAL_SOLVERTHREAD_H
