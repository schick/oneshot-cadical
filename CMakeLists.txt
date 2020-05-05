cmake_minimum_required(VERSION 3.15)
project(oneshot_cadical)

set(CMAKE_CXX_STANDARD 14)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#
## HACK TO REMOVE TARGETS
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)

add_subdirectory(lib/Catch2)
include(lib/Catch2/contrib/Catch.cmake)

add_executable(foo tests/assume.cpp Node.cpp Tree.cpp SolverThread.cpp)
target_link_libraries(foo Catch2::Catch2 ${CMAKE_CURRENT_SOURCE_DIR}/libcadical.a Threads::Threads)

include(CTest)
catch_discover_tests(foo)
#

add_executable(oneshot_cadical main.cpp Node.cpp Node.h Tree.cpp Tree.h SolverThread.cpp SolverThread.h)

target_link_libraries(oneshot_cadical ${CMAKE_CURRENT_SOURCE_DIR}/libcadical.a Threads::Threads)
