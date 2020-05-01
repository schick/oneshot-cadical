cmake_minimum_required(VERSION 3.15)
project(oneshot_cadical)

set(CMAKE_CXX_STANDARD 14)

add_executable(oneshot_cadical main.cpp Node.cpp Node.h Tree.cpp Tree.h SolverThread.cpp SolverThread.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(oneshot_cadical ${CMAKE_CURRENT_SOURCE_DIR}/libcadical.a Threads::Threads)
