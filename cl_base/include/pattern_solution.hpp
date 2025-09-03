#pragma once
#include <blueprint.hpp>
#include <pattern.hpp>


struct PatternSolution{
public:
    std::vector<Blueprint*> blueprints;
    GroupNums groupNums;

    PatternSolution(Scheme scheme);
    PatternSolution(const PatternSolution& other);
    PatternSolution& operator=(const PatternSolution& other);
    ~PatternSolution();
    // bool operator<(const PatternSolution& other) const;
    int remain_groups() const;
    int placed_pattern();
    double get_volume() const;
    inline bool is_complete() const{
        return remain_groups()==0;
    }
    
private:
    


};

using History=std::vector<PatternSolution*>;