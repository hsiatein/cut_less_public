#include "utils.hpp"
#include <deque>
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

Metadata::Metadata(Solution* solution):
total_volume(0),util_volume(0),util_rate(0),parts(0),sheets(solution->solution.size()),cuts(0){
    for(const auto& unit:solution->solution){
        total_volume+=unit.sheet.size.get_volume();
        std::deque<Node*> Q;
        Q.push_back(unit.root);
        while (!Q.empty())
        {
            Node* u=Q[0];
            Q.pop_front();
            switch (u->getType())
            {
            case NodeType::PART:
                parts++;
                util_volume+=u->size.get_volume();
                break;
            case NodeType::CUTLOSS:
                cuts++;
                util_volume+=u->size.get_volume();
                break;
            default:
                break;
            }
            for(auto child:u->childs){
                Q.push_back(child);
            }
        }
    }
    if(total_volume>0) util_rate=util_volume/total_volume;

}

json Metadata::to_json() const{
    json result;
    result["TotalVolume"]=total_volume;
    result["UtilVolume"]=util_volume;
    result["UtilRate"]=util_rate;
    result["PartsNum"]=parts;
    result["SheetsNum"]=sheets;
    result["CutsNum"]=cuts;
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