#pragma once
#include "pattern.hpp"
#include "problem.hpp"
#include "pattern_selector.hpp"
#include "lns.hpp"
#include <pattern_merger.hpp>
#include <solution.hpp>
#include <thread>


struct Solver{
public:
    Solver(Problem* problem);
    ~Solver();
    Solution solve();
    Solutions solve_multi_solution_multi_thread();
    Solutions solve_multi_solution_single_thread();
    Problem* problem;    

private:
    
};