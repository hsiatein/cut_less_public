#include <exist_checker.hpp>

ExistChecker::ExistChecker(){
}

ExistChecker::~ExistChecker(){
}

bool ExistChecker::exist(const Pattern& pattern){
    ExistUnit unit(pattern);
    if(existPatterns.find(unit)!=existPatterns.end()) return true;
    existPatterns.insert(unit);
    return false;
}

ExistUnit::ExistUnit(const Pattern& pattern):size(pattern.top->size.size){
    for(auto pair:pattern.partsNum.partsNum){
        parts.push_back(pair.first);
    }
    std::sort(parts.begin(),parts.end());
    std::sort(size.begin(),size.end());
}
ExistUnit::~ExistUnit(){
}

