#pragma once
#include <string>
#include <array>
#include <stdexcept>
#include <xlnt/xlnt.hpp>
#include "json.hpp"
#include "problem.hpp"
#include <optional>

#include "utils.hpp"

struct RawPartType{
    static int NEXT_ID;  
    int id;
    int width;
    int height;
    int thick;
    bool rotatable;
    int qty;
    std::optional<std::array<double,2>> hardness;
    std::string material;

    RawPartType(int width,int height,int thick,bool rotatable,int qty,std::string material);
    RawPartType(int width,int height,int thick,bool rotatable,int qty,std::array<double,2> hardness,std::string material);
    RawPartType(int width,int height,int thick,bool rotatable,int qty,std::optional<std::array<double,2>> hardness,std::string material);
    std::string to_string();
};

struct RawSheetType{
    static int NEXT_ID;  
    int id;
    int width;
    int height;
    int thick;
    int qty;
    std::optional<std::array<double,2>> hardness;
    std::string material;
    std::string stock;
    std::string stockLocation;

    RawSheetType(int width,int height,int thick,int qty,std::string material,std::string stock,std::string stockLocation);
    RawSheetType(int width,int height,int thick,int qty,std::array<double,2> hardness,std::string material,std::string stock,std::string stockLocation);
    RawSheetType(int width,int height,int thick,int qty,std::optional<std::array<double,2>> hardness,std::string material,std::string stock,std::string stockLocation);
    bool canContain(RawPartType& rawPartType);
    std::string to_string();
};

struct RawProblem{
    std::unordered_map<int,std::vector<RawPartType>> partList;
    std::unordered_map<std::string,std::vector<RawSheetType>> sheetList;
    xlnt::workbook inventory;
    xlnt::workbook ingredient;

    void readInventoryFromXlsx(std::string path);
    void readIngredientFromXlsx(std::string path);
    std::string to_string();
    std::vector<RawPartType*> plids2vecRawPartType(std::vector<int> pl_ids);
    std::vector<Problem> constructProblem(std::vector<int> pl_ids);
};