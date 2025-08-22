#pragma once
#include <pattern_solution.hpp>



struct cl_base_EXPORT StagePatterns{
    Problem* problem;
    std::map<size_t,std::vector<Pattern>> patterns;
    
    inline void new_stage(size_t stage){
        patterns[stage]={};
    }

    StagePatterns(Problem* problem);
    Node* to_node(const PatternNode* patternNode) const;
    json to_json(const PatternNode* patternNode);
    json to_json(const Blueprint* blueprint);
    json to_json(const PatternSolution* solution);

    inline std::vector<Pattern>& operator[](size_t stage){
        return patterns[stage];
    }
    inline const std::vector<Pattern>& operator[](size_t stage) const{
        return patterns.at(stage);
    }
};