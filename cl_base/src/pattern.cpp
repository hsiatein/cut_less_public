#include <pattern.hpp>

// Pattern::Pattern():problem(nullptr),PROBLEM_STRUCT(0),top(nullptr),level(0){
//
// }

Pattern::~Pattern(){
    delete top;
}

Pattern::Pattern(size_t PROBLEM_STRUCT,int width,int height,int thick,Orient next_cut_orient)
:PROBLEM_STRUCT(PROBLEM_STRUCT),top(new Node(Size({width,height,thick},{MAX_INT,MAX_INT,MAX_INT}),{NodeType::CUTLOSS,PROBLEM_STRUCT+1},next_cut_orient)),level(0)
{
    top->size.set(next_cut_orient,{top->size[next_cut_orient].first,0});
    partsNum[PROBLEM_STRUCT+1]=1;
}


// Pattern::Pattern(Problem* problem,const PartType& partType):problem(problem),top(new Node(problem,partType)),level(0)
// {
//     partsNum[partType.id]=1;
// }

Pattern::Pattern(const Problem* problem,const PartType& partType, const int level)
:PROBLEM_STRUCT(problem->STRUCT),top(new Node(partType,{REMAIN[level],REMAIN[level],REMAIN[level]})),level(level){
    partsNum[partType.id]=1;
}

Pattern::Pattern(size_t PROBLEM_STRUCT,const Size& size,Orient next_cut_orient,const int level)
:PROBLEM_STRUCT(PROBLEM_STRUCT),top(new Node(Size(size.size,{MAX_INT,MAX_INT,MAX_INT}),{NodeType::STRUCT,PROBLEM_STRUCT},next_cut_orient)),level(level){

}

Pattern::Pattern(const Pattern& pattern,RotateOrient rotateOrient)
:PROBLEM_STRUCT(pattern.PROBLEM_STRUCT),top(new Node(*(pattern.top))),partsNum(pattern.partsNum),level(pattern.level){
    top->rotate(rotateOrient);
}

Pattern::Pattern(const Pattern& pattern)
:PROBLEM_STRUCT(pattern.PROBLEM_STRUCT),top(new Node(*(pattern.top))),partsNum(pattern.partsNum),level(pattern.level){

}

Pattern& Pattern::operator=(const Pattern& other) {
    if (this != &other) {
        PROBLEM_STRUCT=other.PROBLEM_STRUCT;
        top=new Node(*(other.top));
        partsNum=other.partsNum;
        level=other.level;
    }
    return *this;
}

// Pattern::Pattern(Pattern&& pattern):problem(pattern.problem),top(pattern.top),partsNum(std::move(pattern.partsNum)),level(pattern.level){
//     pattern.top=nullptr;
// }

std::optional<int> Pattern::match(const Pattern& other, const Orient left, const Orient right) const{
    const std::pair<int,int> leftSize = top->size[left];
    const std::pair<int,int> rightSize = other.top->size[right];
    int max=std::max(leftSize.first,rightSize.first);
    if(leftSize.first+leftSize.second>max && rightSize.first+rightSize.second>max) return max;
    return std::nullopt;
}

std::vector<OrientMatch> Pattern::collect_match_1D(const Pattern& other) const{
    std::vector<OrientMatch> result;
    std::vector<Orient> orientSet={Orient::X,Orient::Y,Orient::Z};
    for(const Orient& left:orientSet){
        for(const Orient& right:orientSet){
            auto matchResult=match(other,left,right);
            if(matchResult==std::nullopt) continue;
            result.emplace_back(left,right,matchResult.value());
        }
    }

    return result;
}

std::vector<OrientMatchPair> Pattern::collect_match_2D(const Pattern& other,const std::vector<OrientMatch>& match) const{
    std::vector<OrientMatchPair> result;
    size_t matchesize=match.size();
    for(int i=0;i<matchesize;i++){
        for(int j=i+1;j<matchesize;j++){
            OrientMatch first=match[i];
            OrientMatch second=match[j];
            if(std::get<0>(first)!=std::get<0>(second) && std::get<1>(first)!=std::get<1>(second)){
                result.emplace_back(first,second);
            }

        }
    }

    return result;
}

void Pattern::merge(Pattern& other,Orient orient){
    //logger.log_json("merge_other",other.top->to_json());
    Size newSize=merge_size(*this,other,orient);
    Node* newTop=new Node(newSize,{NodeType::STRUCT,-1},orient);
    newTop->addChild(this->top);
    newTop->addChild(other.top);
    this->top=newTop;
    other.top=nullptr;
    this->partsNum=this->partsNum+other.partsNum;
    //logger.log_json("merge",this->top->to_json());
}

// Pattern Pattern::merge(Pattern& p1,Pattern& p2,Orient orient){
//     Size size=merge_size(p1,p2,orient);
//     Pattern result(p1.problem,size,orient,p1.level);
//     result.top->addChild(p1.top);
//     p1.top=nullptr;
//     result.top->addChild(p2.top);
//     p2.top=nullptr;
//     result.partsNum=p1.partsNum+p2.partsNum;
//     return result;
// }


std::string Pattern::to_string() const{
    std::string result="";
    result+="size:("+std::to_string(top->size.size[0])+","+std::to_string(top->size.size[1])+","+std::to_string(top->size.size[2])+")  ";
    result+="remain:("+std::to_string(top->size.remain[0])+","+std::to_string(top->size.remain[1])+","+std::to_string(top->size.remain[2])+")";
    return result;
}

double Pattern::get_parts_volume() const{
    double result=0;
    std::deque<Node*> Q;
    Q.push_back(top);
    while (!Q.empty())
    {
        Node* u=Q[0];
        Q.pop_front();
        switch (u->getType())
        {
        case NodeType::PART:
            if(u->size.get_volume()<0) logger.log("PART");
            result=result+u->size.get_volume();
            break;
        case NodeType::CUTLOSS:
            if(u->size.get_volume()<0) logger.log("CUTLOSS");
            result=result+u->size.get_volume();
            break;
        default:
            break;
        }
        for(Node* child:u->childs){
            Q.push_back(child);
        }
    }
    return result;
}

Size Pattern::merge_size(const Pattern& p1,const Pattern& p2,const Orient orient){
    Vec3i size;
    Vec3i remain;
    std::vector<Orient> orients={Orient::X,Orient::Y,Orient::Z};
    for(int i=0;i<3;i++){
        std::pair<int,int> pair1=p1.top->size[orients[i]];
        std::pair<int,int> pair2=p2.top->size[orients[i]];
        if(orients[i]!=orient){
            if(pair1.first!=pair2.first) throw cleanAndError("Pattern::merge_size : 合并的part尺寸不同");
            size[i]=pair1.first;
            remain[i]=std::min(pair1.second,pair2.second);
        }
        else{
            size[i]=pair1.first+pair2.first;
            if (pair1.second==MAX_INT || pair2.second==MAX_INT) remain[i]=MAX_INT;
            else remain[i]=pair1.second+pair2.second;
        }
    }
    return Size(size,remain);
}

// void Pattern::refresh(){
//     partsNum=top->count();

// }

bool Pattern::resize(Orient orient,int newSize){
    int increment=newSize-top->size[orient].first;
    if(increment<=0) return true;
    if(increment>top->size[orient].second) return false;
    top->resize(orient,increment);
    return true;
}

void Pattern::resize_force(Orient orient,int newSize){
    int increment=newSize-top->size[orient].first;
    increment=increment-top->size[orient].second;
    top->resize(orient,top->size[orient].second);
    top->resize_force(orient,increment);
}

void Pattern::resize_or_merge(Orient orient,int newSize){
    if(resize(orient,newSize)) return;
    int newStructSize=newSize-top->size[orient].first-std::max(top->size[orient].second,0)-CUT_LOSS;
    if(newStructSize<=0){
        logger.log("resize_force");
        resize_force(orient,newSize);
        return;
    }
    logger.log("resize_merge");
    // std::cout<<newStructSize<<std::endl;
    resize(orient,top->size[orient].first+top->size[orient].second);
    Size size=top->size;
    size.set(orient,{CUT_LOSS,0});
    Pattern cutLoss(PROBLEM_STRUCT,size.size[0],size.size[1],size.size[2],orient);
    // logger.log_json("resize_or_merge",cutLoss.top->to_json());
    // std::cout<<1<<std::endl;
    merge(cutLoss,orient);
    size.set(orient,{newStructSize,0});
    Pattern newStruct(PROBLEM_STRUCT,size,orient,level);
    newStruct.partsNum[PROBLEM_STRUCT]=1;
    merge(newStruct,orient);
}


json Pattern::to_json() const{
    json result;
    result["Tree"]=top->to_json();
    result.merge_patch(partsNum.to_json());
    result["level"]=level;
    return result;
}