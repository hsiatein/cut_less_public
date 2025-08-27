#pragma once
#include <utils.hpp>


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
    explicit Problem(json json);
    Problem& operator=(const Problem& other);
    static Problem from_json(std::string path);
    void addPart(int width,int length,int thick,bool rotatable,int qty);
    void addSheet(int width,int length,int thick,int qty,bool small);
    [[nodiscard]] SheetType get_sheet(size_t id) const;
    [[nodiscard]] json to_json() const;
    [[nodiscard]] json parts_to_json() const;
    [[nodiscard]] json sheets_to_json() const;
    // inline NodeStatus get_node_status(size_t partTypeID) const {
    //     if (partTypeID == STRUCT) {
    //         return {NodeType::STRUCT,partTypeID};
    //     }
    //     if (partTypeID == CUTLOSS) {
    //         return {NodeType::CUTLOSS,partTypeID};
    //     }
    //     return {NodeType::PART,partTypeID};
    // }
};

