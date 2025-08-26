#pragma once
#include <pattern.hpp>
#include <merge_checker.hpp>
#include <exist_checker.hpp>
#include <stage_patterns.hpp>

struct PatternMerger
{
    Problem* problem;
    MergeChecker mergeChecker;
    Timer timer;
    explicit PatternMerger(Problem* problem,Timer timer);
    // PatternMerger(Problem* problem,MergeChecker mergeChecker);

    /// @brief 根据配置文件，生成所有满足要求的模式
    /// @param level 生成的级别，越高级别余量越多
    /// @return 生成的所有模式，按照stage(每个模式包含的零件数量)来存储
    [[nodiscard]] StagePatterns generate_patterns(int level=0) const;
    static std::vector<Pattern> generate_merged_pattern(const Pattern& p1,const Pattern& p2);
    [[nodiscard]] std::vector<Pattern> generate_merged_pattern_with_check(const Pattern& p1,const Pattern& p2) const;
    static std::vector<RotateOrientMatch> matches_to_rotateOrientMatches(const std::vector<OrientMatch>& matches);
    static RotateOrientPair regularizeRotate(Orient orient);
    /// @brief 把当前stage分成可以合并成当前stage的两个stage，例如2->1+1,3->2+1,1+2...
    /// @param i stage
    /// @return 和为i的数对的列表
    static std::vector<std::pair<size_t,size_t>> get_subpattern_pair(size_t i);

};
