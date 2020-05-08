add_executable(foo assume.cpp ../Node.cpp ../Tree.cpp ../SolverThread.cpp)
target_link_libraries(foo Catch2::Catch2 ${CMAKE_CURRENT_SOURCE_DIR}/../libcadical.a Threads::Threads)

include(CTest)
catch_discover_tests(foo)