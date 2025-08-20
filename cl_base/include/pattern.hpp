#pragma once
#include <utils.hpp>
#include "node.hpp"

struct cl_base_EXPORT Pattern
{
public:
    Problem* problem;
    Node* top;
    PartsNum partsNum;
    int level;

    Pattern();
    Pattern(Problem* problem,int width,int height,int thick,Orient next_cut_orient);
    Pattern(Problem* problem,const PartType& partType);
    Pattern(Problem* problem,const Size& size,Orient next_cut_orientconst, int level);
    Pattern(Problem* problem,const PartType& partType,const int level);
    Pattern(const Pattern& pattern,RotateOrient rotateOrient);
    Pattern(const Pattern& pattern);
    Pattern(Pattern&& pattern);
    ~Pattern();
    void refresh();
    std::optional<int> match(const Pattern& other, const Orient left, const Orient right) const;
    std::vector<OrientMatch> collect_match_1D(const Pattern& other) const;
    std::vector<OrientMatchPair> collect_match_2D(const Pattern& other,const std::vector<OrientMatch>& match) const;
    bool resize(Orient orient,int newSize);
    void resize_force(Orient orient,int newSize);
    void resize_or_merge(Orient orient,int newSize);
    void merge(Pattern& other,Orient orient);
    json to_json() const;
    Pattern& operator=(const Pattern& other);
    std::string to_string() const;
    double get_parts_volume() const;

    // static Pattern merge(Pattern& p1,Pattern& p2,Orient orient);
    static Size merge_size(const Pattern& p1,const Pattern& p2,const Orient orient);


private:
    

};

