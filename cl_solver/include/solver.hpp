#pragma once
#include <pattern_selector.hpp>
#include <lns.hpp>
#include <pattern_merger.hpp>
#include <solution.hpp>


class Solver{
public:
    Solver(Problem* problem,SolverConfig config);
    ~Solver();
    Solution solve();
    Solution solve_normal();
    Solution solve_with_pre_process();
    Solutions solve_multi_solution_multi_thread();
    Solutions solve_multi_solution_single_thread();
private:
    Problem* problem;
    SolverConfig config;
};