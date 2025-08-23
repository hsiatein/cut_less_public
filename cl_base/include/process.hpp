#pragma once
#include <pattern_solution.hpp>

struct cl_base_EXPORT Process{
    History history;
    std::vector<RecordOption> operations;
    std::vector<std::vector<RecordOption>> all_options;
    Process();
    Process(const Process& process);
    ~Process();
    void log_solution(PatternSolution* solution);
    void log_operation(Option option);
    void log_options(std::vector<Option> options);
    void print_operation(size_t i) const;
};