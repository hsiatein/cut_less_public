#pragma once
#include <pattern_solution.hpp>


/// @brief 按照stage存储模式的数据结构，key为stage，值为该stage下模式的列表
struct StagePatterns{
    Problem* problem;
    std::map<size_t,std::vector<Pattern>> patterns;
    
    inline void new_stage(size_t stage){
        patterns[stage]={};
    }

    StagePatterns(Problem* problem);
    Node* to_node(const PatternNode* patternNode) const;
    int cal_parts(const PatternSolution* solution) const;
    json to_json(const PatternNode* patternNode);
    json to_json(const Blueprint* blueprint);
    json to_json(const PatternSolution* solution);

    inline std::vector<Pattern>& operator[](size_t stage){
        return patterns[stage];
    }
    inline const std::vector<Pattern>& operator[](size_t stage) const{
        return patterns.at(stage);
    }
    void check_self() const;
    
};