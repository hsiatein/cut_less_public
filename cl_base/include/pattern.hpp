#pragma once
#include <node.hpp>
#include <problem.hpp>

struct Pattern
{
public:
    // Problem* problem;
    size_t PROBLEM_STRUCT;
    Node* top;
    PartsNum partsNum;
    int level;

    // Pattern();
    Pattern(size_t PROBLEM_STRUCT,int width,int height,int thick,Orient next_cut_orient);
    // Pattern(Problem* problem,const PartType& partType);
    Pattern(size_t PROBLEM_STRUCT,const Size& size,Orient next_cut_orientconst,int level);
    Pattern(const Problem* problem,const PartType& partType,int level);
    Pattern(const Pattern& pattern,RotateOrient rotateOrient);
    Pattern(const Pattern& pattern);
    // Pattern(Pattern&& pattern);
    ~Pattern();
    // void refresh();
    std::optional<int> match(const Pattern& other, Orient left, Orient right) const;
    std::vector<OrientMatch> collect_match_1D(const Pattern& other) const;
    // std::vector<OrientMatchPair> collect_match_2D(const Pattern& other,const std::vector<OrientMatch>& match) const;
    bool resize(Orient orient,int newSize);
    void resize_force(Orient orient,int newSize);
    void resize_or_merge(Orient orient,int newSize);
    void merge(Pattern& other,Orient orient);
    json to_json() const;
    Pattern& operator=(const Pattern& other);
    std::string to_string() const;
    double get_parts_volume() const;

    // static Pattern merge(Pattern& p1,Pattern& p2,Orient orient);
    static Size merge_size(const Pattern& p1,const Pattern& p2,Orient orient);


private:
    

};

