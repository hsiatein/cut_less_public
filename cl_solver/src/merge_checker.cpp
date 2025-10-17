#include <merge_checker.hpp>


bool MergeChecker::smaller(const Vec3i& a,const Vec3i& b){
    return a[0]<=b[0] && a[1]<=b[1] && a[2]<=b[2];
};


MergeChecker::MergeChecker(Problem* problem):problem(problem){
    // auto bigger=[](const Vec3i& a,const Vec3i& b){
    //     return a[0]>b[0] && a[1]>b[1] && a[2]>b[2];
    // };
    auto largest=problem->sheets.back().size.size;
    std::sort(largest.begin(),largest.end());
    SheetsSize.push_back(largest);
    bool big=true;
    for(auto sheet=problem->sheets.rbegin();sheet!=problem->sheets.rend();++sheet){
        if(big && sheet->small){
            big=false;
            SheetsSize.clear();
        }
        if(!big && !sheet->small){
            continue;
        }
        Vec3i size=sheet->size.size;
        std::sort(size.begin(),size.end());
        bool add=true;
        for(const auto& sheetSize:SheetsSize){
            if(smaller(size,sheetSize)){
                add=false;
                break;
            }
        }
        if(add) SheetsSize.push_back(size);
    }

}

// /// @brief 检查两个PartsNum合并后是否不超过problem的数量要求
// /// @param left 左PartsNum
// /// @param right 右PartsNum
// /// @return 超过返回false，不超过返回true
// bool MergeChecker::parts_num_fit(const PartsNum& left,const PartsNum& right) const{
//     PartsNum num=left+right;
//     for(const auto& pair:num.partsNum){
//         if(pair.first>=problem->STRUCT) continue;
//         if(pair.second>problem->partsNum[pair.first]){
//             return false;
//         }
//     }
//     return true;
// }

void MergeChecker::add_sheet(const SheetType& sheet){
    Vec3i size=sheet.size.size;
    std::sort(size.begin(),size.end());

}

json MergeChecker::to_json() const{
    json result;
    json sheetsSizeJson=json::array();
    for(auto& size:SheetsSize){
        sheetsSizeJson.push_back(json(size));
    }
    result["SheetsSize"]=sheetsSizeJson;
    return result;
}

bool MergeChecker::size_fit(const Vec3i& size) const{
    Vec3i sortedSize=size;
    std::sort(sortedSize.begin(),sortedSize.end());
    for(const auto& sheet:SheetsSize){
        if(smaller(size,sheet)) return true;
    }
    return false;
}