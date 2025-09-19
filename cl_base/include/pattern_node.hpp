#pragma once
#include <utils.hpp>

using CutOrients=std::array<Orient,3>;
using OptionCost=std::pair<int,double>;


struct PatternNode{
    size_t groupID;
    StageLocation stageLocation;
    Size size;
    PatternNode* parent;
    std::vector<PatternNode*> childs;
    Orient next_cut_orient;
    RotateOrient rotate;


    inline bool is_struct() const{
        return stageLocation.first==0 && stageLocation.second==0;
    }

    inline bool is_cutloss() const{
        return stageLocation.first==0 && stageLocation.second==1;
    }

    inline bool is_pattern() const{
        return stageLocation.first>0;
    }

    inline bool is_nonempty() const{
        return (!childs.empty()) || is_pattern();
    }
    bool has_sibling() const;
    void add_child(PatternNode* patternNode);
    void delete_child(PatternNode* patternNode);
    static void lift_childs(PatternNode* patternNode);
    void set_pattern(size_t groupID,StageLocation stageLocation,const Size& size,RotateOrient rotate);
    double cal_utilization_rate();
    double cal_utilization_volume();
    bool can_contain(const Size& size) const;
    std::vector<CutOrients> match(const Size& size);
    OptionCost evaluate(const Size& size,CutOrients cutOrients,const SolverConfig& config);
    void insert(size_t groupID,StageLocation stageLocation,Size size,RotateOrient rotate,CutOrients cutOrients,const SolverConfig& config);
    PatternNode(Size size,Orient orient);
    PatternNode(Size size);
    PatternNode(size_t groupID,StageLocation stageLocation,Size size);
    PatternNode(const PatternNode& other);
    void split(int length,Orient orient,const SolverConfig& config);
    void reform();
    std::vector<PatternNode*> traverse_mut();
    ~PatternNode();


};

