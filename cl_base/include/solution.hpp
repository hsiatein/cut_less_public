#pragma once
#include <pattern_solution.hpp>
#include <stage_patterns.hpp>

struct cl_base_EXPORT Solution
{
    std::vector<Node*> solution;
    std::map<Node*,SheetType> sheets;
    Solution(const Problem& problem,const StagePatterns& patterns,const PatternSolution& p_solution);
    Solution(const Solution& other);
    ~Solution();
    json to_json() const;
};

struct cl_base_EXPORT Solutions
{
    std::vector<Solution*> solutions;
    Solutions();
    ~Solutions();

    inline void push(Solution* solution){
        solutions.push_back(solution);
    }

    inline size_t size(){
        return solutions.size();
    }

    inline Solution* operator[](size_t i){
        return solutions[i];
    }
};
