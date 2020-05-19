//
// Created by maxi on 30.04.20.
//

#ifndef ONESHOT_CADICAL_SOLVERTHREAD_H
#define ONESHOT_CADICAL_SOLVERTHREAD_H


#include "Tree.h"
#include "Clause.h"

#include "cadical.hpp"

#include <thread>
#include <set>

class SolverThread {

public:

    explicit SolverThread(Tree &tree);

    void start();

    std::thread thread;

    static std::atomic<bool> shouldTerminate;

    void read(const char *path);
    void assume(Node *node);
    int solveLimited(int conflits);

    std::set<Clause> learntClauses;
    CaDiCaL::Solver solver = CaDiCaL::Solver();

    void learn();

private:

    struct ClauseAdder : CaDiCaL::ClauseIterator {
        explicit ClauseAdder(SolverThread &solverThread) : _solverThread{solverThread}{};
        SolverThread &_solverThread;
        bool clause (const std::vector<int> & c) override {
            _solverThread.learn_clause(const_cast<std::vector<int>&>(c), 0);
            return true;
        }
    } clauseAdder;

    void solve();
    int nVars = 0;

    void getNextLeaf();
    Tree &tree;

    Node *currentLeaf = nullptr;

    void learn_unit(int lit);
    void learn_clause(std::vector<int> &clause, int glue);
};


#endif //ONESHOT_CADICAL_SOLVERTHREAD_H
