#pragma once
#include <optional>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <memory>
#include <regex>
#include <map>
#include <set>
#include <chrono>
#include <unordered_set>
#include <tuple>
#include <iostream>
#include "logger.hpp"
#include "hyperparameters.hpp"
#include "random.hpp"
#include <iomanip>

#define MAX_INT 2147483647


enum class cl_base_EXPORT NodeType{
    PART,
    STRUCT,
    LEFTOVER,
    CUTLOSS,
};

enum class cl_base_EXPORT Orient{
    X,
    Y,
    Z,
    NONE,
};

enum class cl_base_EXPORT RotateOrient{
    I,
    X,
    Y,
    Z,
    XY,
    XZ,
};

using OrientPair=std::pair<Orient,Orient>;
using OrientMatch=std::tuple<Orient,Orient,int>;
using OrientMatchPair=std::pair<OrientMatch,OrientMatch>;
using RotateOrientPair=std::pair<RotateOrient,RotateOrient>;
using RotateOrientMatch=std::tuple<RotateOrient,RotateOrient,int>;
using Vec3i=std::array<int,3>;


struct cl_base_EXPORT Size{
    // 0->width, 1->length, 2->thick
    Vec3i size;
    Vec3i remain;

    Size();
    Size(const Vec3i& size);
    Size(const Vec3i& size,int remain);
    Size(const Vec3i& size,const Vec3i& remain);
    Size rotate(RotateOrient rotateOrient) const;
    //bool operator==(const Size& other) const;

    inline bool operator==(const Size& other) const{
        return other.size==size && other.remain==remain;
    }
    std::pair<int,int> operator[](const Orient orient) const;
    void set(const Orient orient,const std::pair<int,int>& value);
    inline double get_volume() const{
        return (double)(size[0])*(double)(size[1])*(double)(size[2]);
    }
    json to_json() const;
    json size_to_json() const;
    std::string to_string() const;
};

struct cl_base_EXPORT PartType{
    size_t id;
    Size size;
    bool rotatable;

    PartType(size_t id,int width,int length,int thick,bool rotatable);
    bool operator==(const PartType& other) const {
        return id==other.id;
    }
    json to_json() const;
};

// struct PartTypeHasher{
//     size_t operator()(const PartType& part) const{
//         return part.id;
//     }
// };

struct cl_base_EXPORT SheetType{
    size_t id;
    Size size;
    bool small;
    SheetType();
    SheetType(size_t id,int width,int length,int thick,bool small);
    inline bool operator==(const SheetType& other) const {
        return id==other.id;
    }
    json to_json() const;
};

// struct SheetTypeHasher{
//     size_t operator()(const SheetType& sheet) const{
//         return sheet.id;
//     }
// };

cl_base_EXPORT inline void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

struct cl_base_EXPORT PartsNum{
    std::map<size_t,int> partsNum;

    int& operator[](const size_t& id);
    PartsNum operator+(const PartsNum& other) const;
    json to_json() const;
    inline bool operator==(const PartsNum& other) const {
        return partsNum == other.partsNum;
    }
    int get(const size_t& id) const;
};


cl_base_EXPORT std::runtime_error cleanAndError(std::string exception);

extern cl_base_EXPORT Logger logger;
extern cl_base_EXPORT Random randomEngine;

cl_base_EXPORT void read_config(std::string path);

using StageLocation=std::pair<size_t,size_t>;
using PatternGroup=std::tuple<PartsNum,std::vector<StageLocation>,int>;
using GroupNum=std::pair<std::vector<StageLocation>,int>;
using Scheme=std::vector<PatternGroup>;
using GroupNums=std::vector<GroupNum>;

cl_base_EXPORT std::string to_string(Orient orient);