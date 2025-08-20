#pragma once
#include "pattern.hpp"
#include "problem.hpp"
#include "pattern_selector.hpp"
#include "lns.hpp"
#include <pattern_merger.hpp>
#include <solution.hpp>


struct cl_solver_EXPORT Solver{
public:
    Solver(Problem* problem);
    ~Solver();
    Solution solve();

    Problem* problem;
    
    PatternSolution* solution;

    

private:
    
};