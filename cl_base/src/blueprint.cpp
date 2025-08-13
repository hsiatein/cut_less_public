#include <blueprint.hpp>



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
