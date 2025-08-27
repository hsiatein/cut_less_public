#include <pattern_node.hpp>
#include <deque>


bool PatternNode::has_sibling() const{
    for(auto v:parent->childs){
        if(v==this) continue;
        if(!v->childs.empty() || v->is_pattern()){
            return true;
        }
    }
    return false;
}

bool PatternNode::can_contain(const Size& size) const{
    Vec3i self=this->size.size;
    Vec3i other=size.size;
    return self[0]>=other[0] && self[1]>=other[1] && self[2]>=other[2];
}

PatternNode::PatternNode(Size size,Orient orient):groupID(MAX_INT),stageLocation(0,1),size(size),parent(nullptr),next_cut_orient(orient),rotate(RotateOrient::I){

}

PatternNode::PatternNode(Size size):groupID(MAX_INT),stageLocation(0,0),size(size),parent(nullptr),next_cut_orient(Orient::NONE),rotate(RotateOrient::I){

}

PatternNode::PatternNode(size_t groupID,StageLocation stageLocation,Size size):groupID(groupID),stageLocation(stageLocation),size(size),parent(nullptr),next_cut_orient(Orient::NONE),rotate(RotateOrient::I){

}

PatternNode::PatternNode(const PatternNode& other):groupID(other.groupID),stageLocation(other.stageLocation),size(other.size),parent(nullptr),next_cut_orient(other.next_cut_orient),rotate(other.rotate){
    for(const auto& child:other.childs){
        PatternNode* new_child=new PatternNode(*child);
        add_child(new_child);
    }
}

PatternNode::~PatternNode(){
    for(auto child:childs){
        delete child;
    }
}

void PatternNode::split(int length,Orient orient){
    if(!is_struct()){
        std::cout<<1;
        throw cleanAndError("PatternNode::split : 不是结构不能分割");
    }
    next_cut_orient=orient;
    Size s1=size;
    Size cl=size;
    Size s2=size;
    auto l=size[orient].first;
    s1.set(orient,{length,0});
    cl.set(orient,{CUT_LOSS,0});
    s2.set(orient,{l-CUT_LOSS-length,0});
    PatternNode* struct1=new PatternNode(s1);
    PatternNode* cutloss=new PatternNode(cl,next_cut_orient);
    PatternNode* struct2=new PatternNode(s2);
    add_child(struct1);
    add_child(cutloss);
    add_child(struct2);
}

void PatternNode::add_child(PatternNode* patternNode){
    childs.push_back(patternNode);
    patternNode->parent=this;
    // lift_childs(patternNode);
}

void PatternNode::delete_child(PatternNode* patternNode){
    auto child=std::find(childs.begin(),childs.end(),patternNode);
    if(child==childs.end()) throw cleanAndError("PatternNode::delete_child");
    delete *child;
    childs.erase(child);
}

void PatternNode::lift_childs(PatternNode* patternNode){
    if(patternNode->next_cut_orient==Orient::NONE || !patternNode->is_struct() || patternNode->next_cut_orient!=patternNode->parent->next_cut_orient) return;
    for(auto child:patternNode->childs){
        patternNode->parent->add_child(child);
    }
    patternNode->childs.clear();
    patternNode->parent->delete_child(patternNode);
}

void PatternNode::set_pattern(size_t groupID,StageLocation stageLocation,const Size& size,RotateOrient rotate){
    if(!is_struct()) throw cleanAndError("PatternNode::set_pattern : 不是结构");
    if(!childs.empty()) throw cleanAndError("PatternNode::set_pattern : 不空");
    if(this->size.size!=size.size){
        // std::cout<<this->size.to_string()<<"\n";
        // std::cout<<size.to_string()<<std::endl;
        throw cleanAndError("PatternNode::set_pattern : 尺寸不匹配");
    }
    next_cut_orient=Orient::NONE;
    this->groupID=groupID;
    this->stageLocation=stageLocation;
    this->rotate=rotate;

}

OptionCost PatternNode::evaluate(const Size& size,CutOrients cutOrients){
    Size new_size=size;
    double result=0;
    double volume=std::pow(this->size.get_volume(),2);
    Size structSize=this->size;
    // std::cout<<to_string(cutOrients[0])<<","<<to_string(cutOrients[1])<<","<<to_string(cutOrients[2])<<"\n";
    for(int i=0;i<3;i++){
        Orient orient=cutOrients[i];
        
        if(orient==Orient::NONE){
            // std::cout<<result<<"\n";
            return {i,result};
        }
        else{
            int l=new_size[orient].first;
            int l_s=structSize[orient].first;
            Size tempSize=structSize;
            tempSize.set(orient,{l_s-CUT_LOSS-l,0});
            structSize.set(orient,{l,0});
            result+=std::pow(tempSize.get_volume(),2)/volume;
        }
    }
    // std::cout<<result<<"\n";
    return {3,result};
}

void PatternNode::insert(size_t groupID,StageLocation stageLocation,Size size,RotateOrient rotate,CutOrients cutOrients){
    PatternNode* current=this;
    // std::cout<<to_string(cutOrients[0])<<to_string(cutOrients[1])<<to_string(cutOrients[2])<<"\n";
    // std::cout<<size.to_string()<<"\n";
    // std::cout<<this->size.to_string()<<std::endl;
    for(int i=0;i<3;i++){
        Orient orient=cutOrients[i];
        if(orient==Orient::NONE){
            break;
        }
        current->split(size[orient].first,orient);
        auto need_lift=current;
        current=current->childs[0];
        // if(current!=nullptr){
        //     lift_childs(current);
        // }
        if(need_lift!=nullptr && need_lift->parent!=nullptr){
            lift_childs(need_lift);
        }
    }
    Size new_size=current->size;
    current->set_pattern(groupID,stageLocation,new_size,rotate);
}

std::vector<CutOrients> PatternNode::match(const Size& size){
    if(!can_contain(size)) return {};
    int noneNum=0;
    int cutNum=0;
    CutOrients cutOrients;
    std::vector<Orient> orients={Orient::X,Orient::Y,Orient::Z};
    for(Orient orient:orients){
        if(size[orient].first+std::max(0,size[orient].second)>=this->size[orient].first){
            cutOrients[2-noneNum]=Orient::NONE;
            noneNum++;
        }
        else{
            cutOrients[cutNum]=orient;
            cutNum++;
        }
    }
    if(cutNum==0 || cutNum==1){
        return {cutOrients};
    }
    else if (cutNum==2)
    {
        std::vector<CutOrients> result(2);
        result[0]={cutOrients[0],cutOrients[1],cutOrients[2]};
        result[1]={cutOrients[1],cutOrients[0],cutOrients[2]};
        return result;
    }
    else{
        std::vector<CutOrients> result(6);
        result[0]={cutOrients[0],cutOrients[1],cutOrients[2]};
        result[1]={cutOrients[0],cutOrients[2],cutOrients[1]};
        result[2]={cutOrients[1],cutOrients[0],cutOrients[2]};
        result[3]={cutOrients[1],cutOrients[2],cutOrients[0]};
        result[4]={cutOrients[2],cutOrients[1],cutOrients[0]};
        result[5]={cutOrients[2],cutOrients[0],cutOrients[1]};
        return result;
    }
    
}

std::vector<PatternNode*> PatternNode::traverse(){
    if(childs.empty()) return {this};
    std::vector<PatternNode*> result;
    std::deque<PatternNode*> Q;
    Q.push_back(this);
    while (!Q.empty())
    {
        auto u=Q[0];
        result.push_back(u);
        Q.pop_front();
        for(auto v:u->childs){
            Q.push_back(v);
        }
    }
    return result;
}

void PatternNode::reform(){
    int length=size[next_cut_orient].first;
    std::vector<PatternNode*> new_childs;
    std::deque<PatternNode*> nonempties;
    std::deque<PatternNode*> cutlosses;
    std::vector<PatternNode*> need_delete;
    
    for(auto child:childs){
        if(child->is_nonempty()){
            nonempties.push_back(child);
            // if(child->is_struct()){
            //     // std::cout<<"childs num: "<<child->childs.size()<<"\n";
            //     std::cout<<"orient: "<<to_string(child->next_cut_orient)<<"\n";
            //     std::cout<<"orient: "<<to_string(child->parent->next_cut_orient)<<"\n";
            //     std::cout<<child<<"\n";
            // }
        }
        else if(child->is_cutloss()){
            cutlosses.push_back(child);
        }
        else{
            need_delete.push_back(child);
        }
    }
    // std::cout<<"childs num: "<<childs.size()<<"\n";
    // std::cout<<"nonempties num: "<<nonempties.size()<<"\n";
    // std::cout<<"cutlosses num: "<<cutlosses.size()<<"\n";
    // std::cout<<"need_delete num: "<<need_delete.size()<<"\n";
    while(!cutlosses.empty()){
        if(!nonempties.empty()){
            auto nonempty=nonempties[0];
            nonempties.pop_front();
            new_childs.push_back(nonempty);
            length=length-nonempty->size[next_cut_orient].first;
            
            auto cutloss=cutlosses[0];
            new_childs.push_back(cutloss);
            length=length-cutloss->size[next_cut_orient].first;
            
        }
        else{
            need_delete.push_back(cutlosses[0]);
        }
        cutlosses.pop_front();
    }
    for(PatternNode* garbage:need_delete){
        delete_child(garbage);
    }
    childs=new_childs;
    Size new_size=size;
    new_size.set(next_cut_orient,{length,0});
    PatternNode* new_struct=new PatternNode(new_size);
    add_child(new_struct);
}

double PatternNode::cal_utilization_rate(){
    double total=size.get_volume();
    double pattern=0;
    std::vector<PatternNode *> childs=traverse();
    for(PatternNode* child:childs){
        if(child->is_pattern()){
            pattern+=child->size.get_volume();
        }
    }
    return pattern/total;
}