#include "config.hpp"
#include <utils.hpp>

PartType::PartType(size_t id,int width,int length,int thick,bool rotatable):
id(id),size({width,length,thick}),rotatable(rotatable){
}

json PartType::to_json() const{
    json j;
    j["ID"]=id;
    j["Size"]=size.size_to_json();
    j["Rotatable"]=rotatable;
    return j;
}

SheetType::SheetType(){

}

SheetType::SheetType(size_t id,int width,int length,int thick,bool small):
id(id),size({width,length,thick}),small(small){
}

json SheetType::to_json() const{
    json j;
    j["ID"]=id;
    j["Size"]=size.size_to_json();
    j["Small"]=small;
    return j;
}

int& PartsNum::operator[](const size_t& id){
    if(partsNum.find(id)==partsNum.end()){
        partsNum[id]=0;
    }
    return partsNum[id];
}

int PartsNum::get(const size_t& id) const{
    auto result=partsNum.find(id);
    if(result==partsNum.end()){
        return 0;
    }
    return result->second;
}

PartsNum PartsNum::operator+(const PartsNum& other) const{
    PartsNum result = *this;
    for (const auto& pair : other.partsNum) {
        result[pair.first] += pair.second;
    }

    return result;
}

json PartsNum::to_json() const{
    json j;
    j["PartsNum"]=partsNum;
    return j;
}

size_t PartsNum::to_num() const{
    size_t result=0;
    for(auto [key,value]:partsNum){
        if(value<0) throw cleanAndError("PartsNum::to_num part "+std::to_string(key)+" num is negative");
        result+=value;
    }
    return result;
}

std::runtime_error cleanAndError(std::string exception){

    return std::runtime_error(exception);
}

Size::Size():size({0,0,0}),remain({0,0,0}){

}

Size::Size(const Vec3i& size):size(size),remain({0,0,0}){

}

Size::Size(const Vec3i& size, int remain):size(size),remain({remain,remain,remain}){

}

Size::Size(const Vec3i& size,const Vec3i& remain):size(size),remain(remain){

}

Size Size::rotate(RotateOrient rotateOrient) const{
    std::array<int,3> order;
    switch (rotateOrient)
    {
    case RotateOrient::X:
        order={0,2,1};
        break;
    case RotateOrient::Y:
        order={2,1,0};
        break;
    case RotateOrient::Z:
        order={1,0,2};
        break;
    case RotateOrient::XY:
        order={1,2,0};
        break;
    case RotateOrient::XZ:
        order={2,0,1};
        break;
    default:
        order={0,1,2};
        break;
    }
    return Size({size[order[0]],size[order[1]],size[order[2]]},{remain[order[0]],remain[order[1]],remain[order[2]]});
}

// bool Size::operator==(const Size& other) const{
//     return other.size==size && other.remain==remain;
// }

std::pair<int,int> Size::operator[](const Orient orient) const{
    switch (orient)
    {
    case Orient::X:
        return {size[0],remain[0]};
    case Orient::Y:
        return {size[1],remain[1]};
    case Orient::Z:
        return {size[2],remain[2]};
    default:
        throw cleanAndError("Size::operator[] : 索引错误");
    }
}

void Size::set(const Orient orient,const std::pair<int,int>& value){
    switch (orient)
    {
    case Orient::X:
        size[0]=value.first;
        remain[0]=value.second;
        break;
    case Orient::Y:
        size[1]=value.first;
        remain[1]=value.second;
        break;
    case Orient::Z:
        size[2]=value.first;
        remain[2]=value.second;
        break;
    default:
        throw cleanAndError("Size::set : 索引错误");
    }
}

json Size::to_json() const{
    json j;
    j["1)Size"]=size;
    j["2)Remain"]=remain;
    return j;
}

std::string Size::to_string() const{
    std::string result="Size:("+std::to_string(size[0])+","+std::to_string(size[1])+","+std::to_string(size[2])+"), Remain:("+std::to_string(remain[0])+","+std::to_string(remain[1])+","+std::to_string(remain[2])+")";
    return result;
}



json Size::size_to_json() const{
    json j=size;
    return j;
}

std::string to_string(Orient orient){
    switch (orient)
    {
    case Orient::X:
        return "X";
    case Orient::Y:
        return "Y";
    case Orient::Z:
        return "Z";
    default:
        return "NONE";
    }
}

Timer::Timer(double time_limit):start(std::chrono::high_resolution_clock::now()),time_limit(time_limit){
}

