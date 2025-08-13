#pragma once
#include "utils.hpp"


struct Problem{
    std::vector<PartType> parts;
    std::vector<SheetType> sheets;
    std::vector<int> partsNum;
    std::vector<int> sheetsNum;
    size_t STRUCT;
    size_t CUTLOSS;
    size_t SHEET_ID;
    Problem();
    Problem(const Problem& other);
    Problem(json json);
    Problem& operator=(const Problem& other);
    static Problem from_json(std::string path);
    void addPart(int width,int length,int thick,bool rotatable,int qty);
    void addSheet(int width,int length,int thick,int qty,bool small);
    SheetType get_sheet(size_t id);
    json to_json() const;
    json parts_to_json() const;
    json sheets_to_json() const;
};

