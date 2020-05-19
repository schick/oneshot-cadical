#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch.hpp>

#include "../SolverThread.h"
#include "../cadical.hpp"

//TEST_CASE("UNSAT") {
//    Tree tree{};
//    SolverThread solverThread{tree};
//    const char *path = "/home/maxi/gates/6s33.cnf";
//    solverThread.read(path);
//    int res = solverThread.solveLimited(1000000);
//    REQUIRE(res == 20);
//}
//
//TEST_CASE("Assume SAT ") {
//    Tree tree{};
//    SolverThread solverThread{tree};
//    const char *path = "/home/maxi/gates/vmpc_32.renamed-as.sat05-1919.cnf";
//    solverThread.read(path);
//
//    Node dummy{};
//    Node node1{&dummy, -179};
//    Node node2{&node1, -980};
//    Node node3{&node2, -565};
//    Node node4{&node3, 105};
//    Node node5{&node4, 783};
//
//    solverThread.assume(&node5);
//    int res = solverThread.solveLimited(1000000);
//    REQUIRE(res == 10);
//}

void foo(int lit) {
    printf("Unit Lit: %d\n", lit);
}

void bar(std::vector <int> &clause, int glue) {
    if (glue <= 2) {
        printf("Clause: ");
        for(int i : clause)
            printf("%d ", i);
        printf("Glue: %d\n", glue);
    }
}

struct ClauseChecker : CaDiCaL::ClauseIterator {
    bool clause (const std::vector<int> & c) override {
        printf("Clause: ");
        for(int i : c)
            printf("%d ", i);
        printf("\n");
        return true;
    }
};

TEST_CASE("Unit Literal Callback") {
    CaDiCaL::Solver solver = CaDiCaL::Solver();
//    solver.add_learned_unit_lit_callback(&foo);
//    solver.add_learned_clause_callback(&bar);

    const char *path = "/home/maxi/gates/vmpc_32.renamed-as.sat05-1919.cnf";
    int nVars;
    solver.read_dimacs(path, nVars, 1);
    solver.assume(-179);
    solver.assume(-980);
    solver.assume(-565);
    solver.assume(105);
    solver.assume(783);

    ClauseChecker cc;
    printf("%ld\n", solver.redundant());
    solver.traverse_red_clauses(cc);
    solver.limit("conflicts", 100);
    solver.solve();
    printf("%ld\n", solver.redundant());
    solver.traverse_red_clauses(cc);
}
