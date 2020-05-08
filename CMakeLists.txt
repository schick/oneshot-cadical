cmake_minimum_required(VERSION 3.10)
project(oneshot_cadical)

set(CMAKE_CXX_STANDARD 14)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if (BUILD_TESTS)
    ## HACK TO REMOVE TARGETS
    set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    # Load test framework
    add_subdirectory(lib/Catch2)
    include(lib/Catch2/contrib/Catch.cmake)
    # Load tests
    add_subdirectory(tests)
endif (BUILD_TESTS)

add_executable(oneshot_cadical main.cpp Node.cpp Node.h Tree.cpp Tree.h SolverThread.cpp SolverThread.h)

target_link_libraries(oneshot_cadical ${CMAKE_CURRENT_SOURCE_DIR}/libcadical.a Threads::Threads)
