#include "node.hpp"
#include "utils.hpp"
#include <algorithm>
#include <deque>
#include <stage_patterns.hpp>
#include <vector>

StagePatterns::StagePatterns(Problem* problem):problem(problem){

}

Node* StagePatterns::to_node(const PatternNode* patternNode) const{
    if(patternNode->is_cutloss()){
        // return new Node(problem,patternNode->size.size[0],patternNode->size.size[1],patternNode->size.size[2],patternNode->next_cut_orient);
        return new Node(Size({patternNode->size.size[0],patternNode->size.size[1],patternNode->size.size[2]}),{NodeType::CUTLOSS,problem->CUTLOSS},patternNode->next_cut_orient);
    }
    if (patternNode->is_struct())
    {
        Node* self=new Node(patternNode->size,{NodeType::STRUCT,problem->STRUCT},patternNode->next_cut_orient);
        for(auto child:patternNode->childs){
            Node* child_node=to_node(child);
            self->addChild(child_node);
        }
        return self;
    }
    Pattern pattern(patterns.at(patternNode->stageLocation.first)[patternNode->stageLocation.second],patternNode->rotate);
    #ifdef DEBUG
    auto old_size=patterns.at(patternNode->stageLocation.first)[patternNode->stageLocation.second].top->size.size;
    auto new_size=pattern.top->size.size;
    std::sort(old_size.begin(),old_size.end());
    std::sort(new_size.begin(),new_size.end());
    if(old_size!=new_size){
        std::cout<<json(old_size).dump()<<", "<<json(new_size).dump()<<std::endl;
        throw cleanAndError("StagePatterns::to_node 尺寸不匹配");
    }
    #endif
    pattern.resize_force(Orient::X,patternNode->size.size[0]);
    pattern.resize_force(Orient::Y,patternNode->size.size[1]);
    pattern.resize_force(Orient::Z,patternNode->size.size[2]);
    Node* top=pattern.top;
    pattern.top=nullptr;
    // #ifdef DEBUG
    // auto node_size=top->size.size;
    // std::sort(node_size.begin(),node_size.end());
    // if(node_size==Vec3i({950,1750,5270})){
    //     Timer timer(0);
    //     timer.print(Color::RED, patternNode->size.to_json().dump(),"\n");
    // }
    // #endif
    
    if(top->size.size!=patternNode->size.size){
        throw cleanAndError("StagePatterns::to_node 变换完尺寸不对");
    }
    return top;
    
}

int StagePatterns::cal_parts(const PatternSolution* solution) const{
    int result=0;
    for(const Blueprint* blueprint:solution->blueprints){
        auto patternNodes=blueprint->top->traverse();
        for(auto patternNode:patternNodes){
            if(patternNode->is_pattern()){
                const auto& partsNum=patterns.at(patternNode->stageLocation.first)[patternNode->stageLocation.second].partsNum;
                for(const auto& [partID,num]:partsNum.partsNum){
                    if(partID!=problem->STRUCT && partID!=problem->CUTLOSS){
                        result+=num;
                    }
                }
            }
        }
    }
    return result;
}

json StagePatterns::to_json(const PatternNode* patternNode){
    Node* node=to_node(patternNode);
    json result=node->to_json();
    delete node;
    return result;
}

json StagePatterns::to_json(const Blueprint* blueprint){
    json result;
    result["Tree"]=to_json(blueprint->top);
    result["SheetID"]=blueprint->sheetID;
    return result;
}

json StagePatterns::to_json(const PatternSolution* solution){
    json result=json::array();
    for(const Blueprint* blueprint:solution->blueprints){
        result.push_back(to_json(blueprint));
    }
    for(const GroupNum& gn:solution->groupNums){
        json j;
        j["Group"]=gn.first;
        j["Num"]=gn.second;
        result.push_back(j);
    }
    return result;
}

Scheme StagePatterns::get_order1_scheme() const{
    Scheme scheme;
    for(int i=0;i<patterns.at(1).size();i++){
        auto pattern=patterns.at(1)[i];
        scheme.emplace_back(pattern.partsNum,std::vector<StageLocation>({std::pair<size_t,size_t>(1,i)}),problem->partsNum[i]);
    }
    return scheme;
}

void StagePatterns::check_self() const{
    for(const auto& [stage,pattern_list]:patterns){
        for(const auto& pattern:pattern_list){
            if(pattern.top==nullptr){
                throw cleanAndError("StagePatterns::check_self(): pattern top is nullptr");
            }
            std::deque<Node*> Q;
            Q.push_back(pattern.top);
            while (!Q.empty())
            {
                Node* u=Q[0];
                Q.pop_front();
                for(auto child:u->childs){
                    Q.push_back(child);
                }
                if(!u->size.valid()){
                    throw cleanAndError("StagePatterns::check_self(): node size is invalid");
                }

            }

        }
    }
}
