#include "utils.hpp"
#include <problem.hpp>


Problem::Problem():STRUCT(0),CUTLOSS(1),SHEET_ID(0){

}

Problem::Problem(const Problem& other):parts(other.parts),sheets(other.sheets),partsNum(other.partsNum),sheetsNum(other.sheetsNum),STRUCT(other.STRUCT),CUTLOSS(other.CUTLOSS),SHEET_ID(other.SHEET_ID){

}

void Problem::addPart(int width,int length,int thick,bool rotatable,int qty){
    parts.emplace_back(STRUCT,width,length,thick,rotatable);
    partsNum.emplace_back(qty);
    STRUCT++;
    CUTLOSS++;
}

void Problem::addSheet(int width,int length,int thick,int qty,bool small){
    sheets.emplace_back(SHEET_ID,width,length,thick,small);
    sheetsNum.emplace_back(qty);
    SHEET_ID++;
}

void Problem::infinity_sheets(){
    for(int i=0;i<sheets.size();i++){
        if(sheets[i].small) sheetsNum[i]=0;
        else sheetsNum[i]=10000000;
    }
}

json Problem::to_json() const{
    json j;
    j["Parts"]=parts_to_json();
    j["Sheets"]=sheets_to_json();
    return j;
}

json Problem::parts_to_json() const{
    json j_array = json::array();
    for(const auto& part:parts){
        json j = part.to_json();
        j["qty"]=partsNum[part.id];
        j_array.push_back(j);
    }
    return j_array;
}

json Problem::sheets_to_json() const{
    json j_array = json::array();
    for(const auto& sheet:sheets){
        json j = sheet.to_json();
        j["qty"]=sheetsNum[sheet.id];
        j_array.push_back(j);
    }
    return j_array;
}

void Problem::check_self() const{
    for(auto sheet:sheets){
        if(sheet.size.size[0]<=sheet.size.size[1] && sheet.size.size[1]<=sheet.size.size[2]) continue;
        else throw cleanAndError("sheet尺寸排序错误");
        
    }
    if(sheets.size()!=sheetsNum.size() || sheets.size()!=need_rotates.size()) throw cleanAndError("向量尺寸错误");
}

RotateOrient sort_size(Vec3i& size){
    RotateOrient result;
    if(size[0]<=size[1] && size[1]<=size[2]){
        return RotateOrient::I;
    }
    else if(size[0]<=size[2] && size[2]<=size[1]){
        size=Size(size).rotate(RotateOrient::X).size;
        return RotateOrient::X;
    }
    else if(size[1]<=size[0] && size[0]<=size[2]){
        size=Size(size).rotate(RotateOrient::Z).size;
        return RotateOrient::Z;
    }
    else if(size[1]<=size[2] && size[2]<=size[0]){
        size=Size(size).rotate(RotateOrient::XY).size;
        return RotateOrient::XZ;
    }
    else if(size[2]<=size[0] && size[0]<=size[1]){
        size=Size(size).rotate(RotateOrient::XZ).size;
        return RotateOrient::XY;
    }
    else{
        size=Size(size).rotate(RotateOrient::Y).size;
        return RotateOrient::Y;
    }
}

Problem::Problem(json json):STRUCT(0),CUTLOSS(1),SHEET_ID(0){
    for(const auto& part:json["Parts"]){
        // std::cout<<part["Rotatable"].dump()<<std::endl;
        int x=std::stoi(part["Size"][0].dump());
        int y=std::stoi(part["Size"][1].dump());
        int z=std::stoi(part["Size"][2].dump());
        if(x<=0 || y<=0 || z<=0) continue;
        addPart(x,y,z,part["Rotatable"].dump()=="true",std::stoi(part["qty"].dump()));
    }
    for(const auto& sheet:json["Sheets"]){
        Vec3i size={std::stoi(sheet["Size"][0].dump()),std::stoi(sheet["Size"][1].dump()),std::stoi(sheet["Size"][2].dump())};
        // std::sort(size.begin(),size.end());
        auto o=sort_size(size);
        need_rotates.push_back(o);
        addSheet(size[0],size[1],size[2],std::stoi(sheet["qty"].dump()),sheet["Small"].get<bool>());
    }
    std::sort(sheets.begin(),sheets.end(),[](const SheetType& a, const SheetType& b) {
        return a.size.get_volume() < b.size.get_volume();
    });
    // for(const auto& sheet:json["Sheets"]){
    //     // std::cout<<sheet["Size"][0].dump()<<std::endl;
    //     addSheet(std::stoi(sheet["Size"][0].dump()),std::stoi(sheet["Size"][1].dump()),std::stoi(sheet["Size"][2].dump()),std::stoi(sheet["qty"].dump()));
    // }
    // logger.log_json("sortedSheets",this->to_json());

}

Problem Problem::from_json(std::string path){
    std::ifstream f(path);
    json data = json::parse(f);
    return Problem(data);
}

Problem& Problem::operator=(const Problem& other) {
    if (this != &other) {
        parts = other.parts;
        sheets = other.sheets;
        partsNum = other.partsNum;
        sheetsNum = other.sheetsNum;
        STRUCT = other.STRUCT;
        CUTLOSS = other.CUTLOSS;
        SHEET_ID = other.SHEET_ID;
    }
    return *this;
}

SheetType Problem::get_sheet(size_t id) const{
    for(auto sheet:sheets){
        if(sheet.id==id) return sheet;
    }
    throw cleanAndError("Problem::get_sheet");
}