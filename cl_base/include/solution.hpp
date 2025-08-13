#pragma once
#include <pattern_solution.hpp>
#include <stage_patterns.hpp>

struct Solution
{
    std::vector<Node*> solution;
    std::map<Node*,SheetType> sheets;
    Solution(Problem& problem,StagePatterns& patterns,PatternSolution& p_solution);
    ~Solution();
    json to_json() const;
};
