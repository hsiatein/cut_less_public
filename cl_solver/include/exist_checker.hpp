#pragma once
#include <pattern.hpp>
#include <unordered_set>

struct ExistUnit
{
    Vec3i size;
    std::vector<size_t> parts;
    ExistUnit(const Pattern& pattern);
    ~ExistUnit();
};

struct ExistUnitHash {
    std::size_t operator()(const ExistUnit& unit) const {
        std::size_t hash = std::hash<int>()(unit.size[0]) ^ std::hash<int>()(unit.size[1]) ^ std::hash<int>()(unit.size[2]);
        for (const auto& part : unit.parts) {
            hash ^= std::hash<size_t>()(part) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

struct ExistUnitEqual {
    bool operator()(const ExistUnit& lhs, const ExistUnit& rhs) const {
        return lhs.size == rhs.size && lhs.parts == rhs.parts; // parts 已经排过序
    }
};

class ExistChecker
{
private:
    std::unordered_set<ExistUnit,ExistUnitHash,ExistUnitEqual> existPatterns;
public:
    ExistChecker();
    bool exist(const Pattern& pattern);
    ~ExistChecker();
};
