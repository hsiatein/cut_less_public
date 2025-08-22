#pragma once
#include "pattern.hpp"
#include <Highs.h>
#include <stage_patterns.hpp>


struct PartsNumHasher {
    std::size_t operator()(const PartsNum& p) const {
        std::size_t hash = 0;
        for (const auto& [key, value] : p.partsNum) {
            hash ^= std::hash<size_t>()(key) ^ (std::hash<int>()(value) << 1);
        }
        return hash;
    }
};

class cl_solver_EXPORT PatternSelector{
public:
    PatternSelector(Problem* problem);
    void partsnum_register(const StagePatterns& patterns,size_t max_stage);
    void partsnum_register(const StagePatterns& patterns);
    json to_json() const;
    json scheme_to_json(const Scheme& scheme) const;
    size_t get_patterns_size() const;
    Scheme select();
    std::tuple<std::vector<HighsInt>,std::vector<HighsInt>,std::vector<double>> generate_highs_mat();
    std::vector<PartsNum> partsnums;
private:
    Problem* problem;
    int patterns_size;
    std::unordered_map<PartsNum,std::vector<StageLocation>,PartsNumHasher> partsnum_of_patterns;
    HighsModel model;
    Highs highs;
};

