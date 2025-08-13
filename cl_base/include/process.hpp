#pragma once
#include <pattern_solution.hpp>

struct Process{
    History history;
    std::vector<Option> operations;
    Process();
    Process(const Process& process);
    ~Process();
    void log_solution(PatternSolution* solution);
    void log_option(Option option);
};