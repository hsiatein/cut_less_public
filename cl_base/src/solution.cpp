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
        solution.emplace_back(problem.get_sheet(blueprint->sheetID),node);
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

Solutions::Solutions(){

}

Solutions::~Solutions(){
    for(Solution* solution:solutions){
        delete solution;
    }
}