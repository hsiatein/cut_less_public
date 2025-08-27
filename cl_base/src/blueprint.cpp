#include <blueprint.hpp>
#include <deque>



Blueprint::Blueprint(const SheetType& sheet):sheetID(sheet.id),top(new PatternNode(sheet.size)){

}

Blueprint::Blueprint(const Blueprint& other):sheetID(other.sheetID),top(new PatternNode(*other.top)){

}

Blueprint::~Blueprint(){
    emptyStructs.clear();
    delete top;
}

std::vector<PatternNode*> Blueprint::get_empty_structs(){
    if(emptyStructs.empty()){
        std::deque<PatternNode*> Q;
        Q.push_back(top);
        while(!Q.empty()){
            PatternNode* u=Q[0];
            Q.pop_front();
            if(!u->is_struct()) continue;
            if(u->childs.empty()){
                emptyStructs.push_back(u);
            }
            else{
                for(auto child:u->childs){
                    Q.push_back(child);
                }
            }

        }
    }
    return emptyStructs;
}

double Blueprint::get_volume() const{
    return top->size.get_volume();
}

std::string to_string(const Option& op){
    std::string result="Sheet: "+std::to_string(std::get<0>(op)->sheetID)+", Space: "+std::get<1>(op)->size.to_string()+"\nGroupID: "+std::to_string(std::get<2>(op))+", Size: "+std::get<4>(op).to_string()+"\nCut Orient: ";
    for(auto o:std::get<6>(op)){
        result+=to_string(o)+" ";
    }
    result+="Cost: ("+std::to_string(std::get<7>(op).first)+", "+std::to_string(std::get<7>(op).second)+")";
    return result;
}

std::string to_string(const RecordOption& op){
    std::string result="Sheet: "+std::to_string(std::get<0>(op))+", Space: "+std::get<1>(op).size_to_json().dump()+"\nGroupID: "+std::to_string(std::get<2>(op))+", Size: "+std::get<3>(op).to_string()+"\nCut Orient: ";
    for(auto o:std::get<4>(op)){
        result+=to_string(o)+" ";
    }
    result+="Cost: ("+std::to_string(std::get<5>(op).first)+", "+std::to_string(std::get<5>(op).second)+")";
    return result;
}