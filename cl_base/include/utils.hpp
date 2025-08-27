#pragma once
#include <array>
#include <chrono>
#include <tuple>
#include <iostream>
#include <logger.hpp>
#include <random.hpp>


#define MAX_INT 2147483647


enum class NodeType{
    PART,
    STRUCT,
    LEFTOVER,
    CUTLOSS,
};

using NodeStatus=std::pair<NodeType,size_t>;

enum class Orient{
    X,
    Y,
    Z,
    NONE,
};

enum class RotateOrient{
    I,
    X,
    Y,
    Z,
    XY,
    XZ,
};

enum class Color{
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    CYAN,
    RED,
    NONE,
};

using OrientPair=std::pair<Orient,Orient>;
using OrientMatch=std::tuple<Orient,Orient,int>;
using OrientMatchPair=std::pair<OrientMatch,OrientMatch>;
using RotateOrientPair=std::pair<RotateOrient,RotateOrient>;
using RotateOrientMatch=std::tuple<RotateOrient,RotateOrient,int>;
using Vec3i=std::array<int,3>;


struct Size{
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

struct PartType{
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

struct SheetType{
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

inline void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

struct PartsNum{
    std::map<size_t,int> partsNum;

    int& operator[](const size_t& id);
    PartsNum operator+(const PartsNum& other) const;
    json to_json() const;
    inline bool operator==(const PartsNum& other) const {
        return partsNum == other.partsNum;
    }
    int get(const size_t& id) const;
};


std::runtime_error cleanAndError(std::string exception);

extern Logger logger;
extern Random randomEngine;

void read_config(std::string path);
void read_config_json(json config);

using StageLocation=std::pair<size_t,size_t>;
using PatternGroup=std::tuple<PartsNum,std::vector<StageLocation>,int>;
using GroupNum=std::pair<std::vector<StageLocation>,int>;
using Scheme=std::vector<PatternGroup>;
using GroupNums=std::vector<GroupNum>;

std::string to_string(Orient orient);

struct Timer{
public:
    Timer();
    /// @brief 检查是否超时
    /// @param limit 时间限制
    /// @return 是否超时
    inline bool is_overtime(double limit=TIME_LIMIT) const{
        return get_runtime()>=limit;
    }

    /// @brief 获取运行时间
    /// @return 运行时间（ms）
    inline double get_runtime() const{
        std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - start;
        return duration.count();
    }

    /// @brief 获取剩余时间
    /// @return 剩余时间（ms）
    inline double get_remain_time() const{
        return TIME_LIMIT-get_runtime();
    }

    /// @brief 根据颜色打印字符串
    /// @param color 
    /// @param str 要打印的字符串
    template <typename... Args>
    void print(Color color, const Args&... str) const{
        std::string color_str="\033[0m";
        switch (color)
        {
        case Color::BLUE:
            color_str="\033[34m";
            break;
        case Color::GREEN:
            color_str="\033[32m";
            break;
        case Color::YELLOW:
            color_str="\033[33m";
            break;
        case Color::PURPLE:
            color_str="\033[35m";
            break;
        case Color::CYAN:
            color_str="\033[36m";
            break;
        case Color::RED:
            color_str="\033[31m";
            break;
        default:
            break;
        }
        std::cout << color_str;
        print_str(str...);
        std::cout << "\033[0m";
    }

    /// @brief 打印当前runtime
    /// @param color 颜色
    inline void print_time(Color color) const{
        print(color,std::fixed,std::setprecision(3),"[",get_runtime()," ms]");
    }
private:
    // std::chrono::_V2::system_clock::time_point start;
    #ifdef __linux__
    std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration> start;
    #else
    #ifdef __APPLE__
    std::chrono::steady_clock::time_point start;
    #else
    std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration> start;
    #endif
    #endif

    template <typename T>
    void print_str(const T& str) const{
        std::cout << str;
    }

    template <typename T, typename... Args>
    void print_str(const T& first, const Args&... rest) const{
        std::cout << first;
        print_str(rest...);
    }
};

