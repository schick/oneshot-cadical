#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch.hpp>

#include "../SolverThread.h"

TEST_CASE("UNSAT") {
    Tree tree{};
    SolverThread solverThread{tree};
    const char *path = "/home/maxi/gates/6s33.cnf";
    solverThread.read(path);
    int res = solverThread.solveLimited(1000000);
    REQUIRE(res == 20);
}

TEST_CASE("Assume SAT ") {
    Tree tree{};
    SolverThread solverThread{tree};
    const char *path = "/home/maxi/gates/vmpc_32.renamed-as.sat05-1919.cnf";
    solverThread.read(path);

    Node dummy{};
    Node node1{&dummy, -179};
    Node node2{&node1, -980};
    Node node3{&node2, -565};
    Node node4{&node3, 105};
    Node node5{&node4, 783};

    solverThread.assume(&node5);
    int res = solverThread.solveLimited(1000000);
    REQUIRE(res == 10);
}