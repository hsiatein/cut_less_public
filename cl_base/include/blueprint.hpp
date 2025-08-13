#pragma once
#include "pattern_node.hpp"


struct Blueprint
{
    size_t sheetID;
    PatternNode* top;
    std::vector<PatternNode*> emptyStructs;
    Blueprint(const SheetType& sheet);
    Blueprint(const Blueprint& other);
    ~Blueprint();
    std::vector<PatternNode*> get_empty_structs();
    double get_volume() const;


};

using Option=std::tuple<Blueprint*,PatternNode*,size_t,StageLocation,Size,RotateOrient,CutOrients,OptionCost>;

