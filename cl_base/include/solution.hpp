#pragma once
#include "utils.hpp"
#include <pattern_solution.hpp>
#include <stage_patterns.hpp>

struct SolutionUnit
{
    SheetType sheet;
    Node* root;
    
    SolutionUnit(SheetType sheet,Node* root);
    SolutionUnit(const SolutionUnit& other);
    SolutionUnit(SolutionUnit&& other);
    ~SolutionUnit();
    json to_json() const;
};

struct Solution
{
    std::vector<SolutionUnit> solution;
    Solution();
    Solution(const Problem& problem,const StagePatterns& patterns,const PatternSolution& p_solution);
    Solution(const Solution& other);
    void merge(Solution& other);
    ~Solution();
    json to_json() const;
};

struct Solutions
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
