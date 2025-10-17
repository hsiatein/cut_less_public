#pragma once
#include <problem.hpp>


class MergeChecker{
public:
    MergeChecker(Problem* problem);
    // bool parts_num_fit(const PartsNum& left,const PartsNum& right) const;
    bool size_fit(const Vec3i& size) const;
    json to_json() const;

private:
    Problem* problem;
    std::vector<Vec3i> SheetsSize;
    void add_sheet(const SheetType& sheet);
    static bool smaller(const Vec3i& a,const Vec3i& b);
};
