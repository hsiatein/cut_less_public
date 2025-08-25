#pragma once
#include <pattern.hpp>
#include <merge_checker.hpp>
#include <exist_checker.hpp>
#include <stage_patterns.hpp>

struct PatternMerger
{
    Problem* problem;
    MergeChecker mergeChecker;
    PatternMerger(Problem* problem);
    PatternMerger(Problem* problem,MergeChecker mergeChecker);
    // inline Pattern& get_pattern(StageLocation location){
    //     return patterns[location.first][location.second];
    // }
    StagePatterns generate_patterns();
    static std::vector<Pattern> generate_merged_pattern(const Pattern& p1,const Pattern& p2);
    static std::vector<Pattern> generate_merged_pattern_with_check(const MergeChecker& mergeCheck,const Pattern& p1,const Pattern& p2);
    static std::vector<RotateOrientMatch> matches_to_rotateOrientMatches(const std::vector<OrientMatch>& matches);
    static RotateOrientPair regularizeRotate(Orient orient);
    std::vector<std::pair<size_t,size_t>> get_subpattern_pair(size_t i);

};
