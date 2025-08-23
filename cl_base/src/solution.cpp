#include <solution.hpp>

Solution::Solution(const Problem& problem,const StagePatterns& patterns,const PatternSolution& p_solution){
    for(const auto blueprint:p_solution.blueprints){
        Node* node=patterns.to_node(blueprint->top);
        solution.push_back(node);
        sheets[node]=problem.get_sheet(blueprint->sheetID);
    }
}

Solution::Solution(const Solution& other){
    for(auto other_node:other.solution){
        Node* node=new Node(*other_node);
        solution.push_back(node);
        sheets[node]=other.sheets.at(other_node);
    }
}

Solution::~Solution(){
    for(auto node:solution){
        delete node;
    }
}

json Solution::to_json() const{
    json result=json::array();
    for(auto node:solution){
        result.push_back(node->to_json());
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