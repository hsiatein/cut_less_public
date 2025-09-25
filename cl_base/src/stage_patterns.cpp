#include <stage_patterns.hpp>

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
        throw cleanAndError("尺寸不匹配");
    }
    #endif
    pattern.resize_force(Orient::X,patternNode->size.size[0]);
    pattern.resize_force(Orient::Y,patternNode->size.size[1]);
    pattern.resize_force(Orient::Z,patternNode->size.size[2]);
    Node* top=pattern.top;
    pattern.top=nullptr;
    return top;
    
}

int StagePatterns::cal_parts(const PatternSolution* solution) const{
    int result=0;
    for(const Blueprint* blueprint:solution->blueprints){
        auto patternNodes=blueprint->top->traverse_mut();
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