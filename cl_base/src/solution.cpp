#include "utils.hpp"
#include <solution.hpp>
#include <utility>

SolutionUnit::SolutionUnit(SheetType sheet,Node* root):sheet(sheet),root((root)){
}
SolutionUnit::SolutionUnit(const SolutionUnit& other):sheet(other.sheet),root(new Node(*other.root)){

}
SolutionUnit::SolutionUnit(SolutionUnit&& other):sheet(other.sheet),root(other.root){
    other.root=nullptr;
}
SolutionUnit::~SolutionUnit(){
    delete root;
}

json SolutionUnit::to_json() const{
    json result;
    result["Sheet"]=sheet.to_json();
    result["Root"]=root->to_json();
    return result;
}

Solution::Solution(){
    
}

Solution::Solution(const Problem& problem,const StagePatterns& patterns,const PatternSolution& p_solution){
    for(const auto blueprint:p_solution.blueprints){
        Node* node=patterns.to_node(blueprint->top);
        SheetType sheet=problem.get_sheet(blueprint->sheetID);
        node->rotate(problem.need_rotates[sheet.id]);
        sheet.size=sheet.size.rotate(problem.need_rotates[sheet.id]);
        solution.emplace_back(sheet,node);
    }
}

Solution::Solution(const Solution& other){
    for(auto other_node:other.solution){
        solution.emplace_back(other_node);
    }
}

void Solution::merge(Solution& other){
    for(auto other_node:other.solution){
        solution.push_back(std::move(other_node));
    }
}

Solution::~Solution(){
}

json Solution::to_json() const{
    json result=json::array();
    for(auto node:solution){
        result.push_back(node.to_json());
    }
    return result;
}

void Solution::check_self(const Timer& timer) const{
    for(const auto& unit:solution){
        if(unit.root==nullptr) throw cleanAndError("Solution::check_self(): root is nullptr");
        if(!unit.root->valid(timer)) throw cleanAndError("Solution::check_self(): root size is invalid");
    }
}

Solutions::Solutions(){

}

Solutions::~Solutions(){
    for(Solution* solution:solutions){
        delete solution;
    }
}