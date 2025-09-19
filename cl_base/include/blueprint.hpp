#pragma once
#include <pattern_node.hpp>


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
    double get_util_volume() const;

};

/// @brief blueprint, pattern node, group ID, stage location, pattern size, rotate, cut orient, cost
using Option=std::tuple<Blueprint*,PatternNode*,size_t,StageLocation,Size,RotateOrient,CutOrients,OptionCost>;
/// @brief blueprint sheet id, pattern node size, group ID, pattern size, cut orient, cost
using RecordOption=std::tuple<size_t,Size,size_t,Size,CutOrients,OptionCost>;
std::string to_string(const Option& op);
std::string to_string(const RecordOption& op);