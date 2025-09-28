#pragma once
#include <blueprint.hpp>
#include <pattern.hpp>


struct PatternSolution{
public:
    std::vector<Blueprint*> blueprints;
    GroupNums groupNums;
    int parts_num=0;

    PatternSolution(Scheme scheme);
    PatternSolution(const PatternSolution& other);
    PatternSolution& operator=(const PatternSolution& other);
    ~PatternSolution();
    // bool operator<(const PatternSolution& other) const;
    int remain_groups() const;
    int placed_pattern() const;
    double get_volume() const;
    inline bool is_complete() const{
        return remain_groups()==0;
    }
    double cal_util_rate() const;
    void check_self() const;
private:
    


};

using History=std::vector<PatternSolution*>;