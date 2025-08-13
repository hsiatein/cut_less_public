#include "../include/node.hpp"

Node::Node():problem(nullptr),partTypeID(0),next_cut_orient(Orient::NONE){
}

Node::Node(Problem* problem,int width,int height,int thick,size_t partTypeID,Node* parent,Orient next_cut_orient)
:problem(problem),size({width,height,thick},REMAIN[0]),partTypeID(partTypeID),parent(parent),next_cut_orient(next_cut_orient)
{

}

Node::Node(Problem* problem,int width,int height,int thick,Orient next_cut_orient)
:problem(problem),size({width,height,thick},{MAX_INT,MAX_INT,MAX_INT}),partTypeID(problem->CUTLOSS),parent(nullptr),next_cut_orient(next_cut_orient)
{
    size.set(next_cut_orient,{size[next_cut_orient].first,0});
}

Node::Node(Problem* problem,int width,int height,int thick,Orient next_cut_orient,const Vec3i& remain)
:problem(problem),size({width,height,thick},remain),partTypeID(problem->STRUCT),parent(nullptr),next_cut_orient(next_cut_orient)
{

}

Node::Node(Problem* problem,const Size& size,int partTypeID,Orient next_cut_orient)
:problem(problem),size(size),partTypeID(partTypeID),parent(nullptr),next_cut_orient(next_cut_orient){

}

Node::Node(Problem* problem,const PartType& partType)
:problem(problem),size(partType.size),partTypeID(partType.id),parent(nullptr),next_cut_orient(Orient::NONE)
{
    size.remain={REMAIN[0],REMAIN[0],REMAIN[0]};
}
Node::Node(Problem* problem,const PartType& partType,const Vec3i& remain)
:problem(problem),size(partType.size.size,remain),partTypeID(partType.id),parent(nullptr),next_cut_orient(Orient::NONE)
{
}

Node::Node(const Node& node)
:problem(node.problem),size(node.size),partTypeID(node.partTypeID),parent(node.parent),next_cut_orient(node.next_cut_orient)
{
    for(Node* child:node.childs){
        Node* newChild=new Node(*child);
        this->addChild(newChild);
    }   
}

Node::Node(Node&& other)
:problem(other.problem),size(other.size),partTypeID(other.partTypeID),parent(other.parent),next_cut_orient(other.next_cut_orient)
{
    for(auto want_to_move:other.childs){
        addChild(want_to_move);
    }
    other.parent = nullptr;
    other.childs.clear();
}

Node::~Node(){
    for(auto ptr:this->childs){
        delete ptr;
    }
}

void Node::deleteChild(Node* child){
    auto it=std::find(childs.begin(), childs.end(), child);
    if(it==childs.end()) throw cleanAndError("Node::deleteChild : 传入节点不为子节点");
    childs.erase(it);
    delete child;
}

bool Node::containChild(const Node* child){
    return std::find(childs.begin(), childs.end(), child)!=childs.end();
}

void Node::addChild(Node* child){
    if(child==nullptr) throw cleanAndError("Node::addChild : 传入子节点为空");
    if(containChild(child)) throw cleanAndError("Node::addChild : 传入节点已经是子节点");
    child->parent=this;
    this->childs.push_back(child);
    if(child->getType()==NodeType::STRUCT && this->next_cut_orient==child->next_cut_orient){
        liftChild(child);
    }
}

void Node::addChild(int width,int height,int thick,size_t partTypeID,Orient next_cut_orient){
    Node* child=new Node(problem,width,height,thick,partTypeID,nullptr,next_cut_orient);
    addChild(child);
}

void Node::addChild(PartType partType){
    Node* child=new Node(problem,partType);
    addChild(child);
}

void Node::addChild(const Node& node){
    Node* child=new Node(node);
    addChild(child);
}

void Node::liftChild(Node* child){
    for(auto want_to_lift:child->childs){
        addChild(want_to_lift);
    }
    child->childs.clear();
    deleteChild(child);
}

void Node::rotate(RotateOrient rotateOrient){
    size=size.rotate(rotateOrient);
    switch (rotateOrient)
    {
    case RotateOrient::X:
        if(next_cut_orient==Orient::Y) next_cut_orient=Orient::Z;
        else if(next_cut_orient==Orient::Z)  next_cut_orient=Orient::Y;
        break;
    case RotateOrient::Y:
        if(next_cut_orient==Orient::X) next_cut_orient=Orient::Z;
        else if(next_cut_orient==Orient::Z)  next_cut_orient=Orient::X;
        break;
    case RotateOrient::Z:
        if(next_cut_orient==Orient::X) next_cut_orient=Orient::Y;
        else if(next_cut_orient==Orient::Y)  next_cut_orient=Orient::X;
        break;
    case RotateOrient::XY:
        if(next_cut_orient==Orient::X) next_cut_orient=Orient::Z;
        else if(next_cut_orient==Orient::Y)  next_cut_orient=Orient::X;
        else if(next_cut_orient==Orient::Z)  next_cut_orient=Orient::Y;
        break;
    case RotateOrient::XZ:
        if(next_cut_orient==Orient::X) next_cut_orient=Orient::Y;
        else if(next_cut_orient==Orient::Y)  next_cut_orient=Orient::Z;
        else if(next_cut_orient==Orient::Z)  next_cut_orient=Orient::X;
        break;
    default:
        return;
    }

    for(auto child:childs){
        child->rotate(rotateOrient);
    }
}

PartsNum Node::count(){
    std::deque<Node*> openList;
    PartsNum result;
    openList.push_back(this);
    while(!openList.empty()){
        Node* u=openList[0];
        openList.pop_front();
        for(Node* v:u->childs){
            openList.push_back(v);
        }
        if(u->partTypeID==problem->STRUCT && !u->childs.empty()) continue;
        result[u->partTypeID]++;
    }
    return result;
}

NodeType Node::getType() const{
    if(partTypeID==problem->STRUCT){
        if(childs.empty()) return NodeType::LEFTOVER;
        else return NodeType::STRUCT;
    }
    else if(partTypeID==problem->CUTLOSS){
        return NodeType::CUTLOSS;
    }
    else if(partTypeID<problem->STRUCT && partTypeID>=0){
        return NodeType::PART;
    }
    else throw cleanAndError("Node::getType : ID错误");
}

json Node::to_json() const{
    json result;
    // json sizeJson=size.to_json();
    // result["Size"]=sizeJson["Size"];
    // result["Remain"]=sizeJson["Remain"];
    // result.push_back(size.to_json());
    std::string field3="3)NodeType";
    switch (getType())
    {
    case NodeType::STRUCT:
        result.merge_patch(size.to_json());
        result[field3]="Struct";
        break;
    case NodeType::LEFTOVER:
        result["1)Size"]=size.size_to_json();
        result[field3]="Leftover";
        break;
    case NodeType::CUTLOSS:
        result["1)Size"]=size.size_to_json();
        result[field3]="Cutloss";
        break;
    case NodeType::PART:
        result.merge_patch(size.to_json());
        result[field3]="Part:"+std::to_string(partTypeID);
        break;
    default:
        break;
    }
    std::string field4="4)Orient";
    switch (next_cut_orient)
    {
    case Orient::NONE:
        result[field4]="None";
        break;
    case Orient::X:
        result[field4]="X";
        break;
    case Orient::Y:
        result[field4]="Y";
        break;
    case Orient::Z:
        result[field4]="Z";
        break;
    default:
        break;
    }
    json j_childs=json::array();
    for(const auto& child:childs){
        j_childs.push_back(child->to_json());
    }
    result["5)Childs"]=j_childs;

    return result;
}

void Node::resize(Orient orient,int increment){
    if(increment==0) return;
    NodeType type=getType();
    std::pair<int,int> newSize=size[orient];
    newSize.first=newSize.first+increment;
    if(type==NodeType::PART){
        newSize.second=newSize.second-increment;
        size.set(orient,newSize);
    }else if(type==NodeType::CUTLOSS || type==NodeType::LEFTOVER){
        size.set(orient,newSize);
    }else if(type==NodeType::STRUCT && orient!=next_cut_orient){
        newSize.second=newSize.second-increment;
        size.set(orient,newSize);
        for(auto& child:childs){
            child->resize(orient,increment);
        }
    }else if(type==NodeType::STRUCT && orient==next_cut_orient){
        newSize.second=newSize.second-increment;
        size.set(orient,newSize);
        int allRemain=increment;
        for(auto& child:childs){
            NodeType childType=child->getType();
            if(childType==NodeType::CUTLOSS){
                continue;
            }
            int remain=std::max(child->size[orient].second,0);
            if(childType==NodeType::LEFTOVER || remain>=allRemain){
                child->resize(orient,allRemain);
                break;
            }else{
                allRemain=allRemain-remain;
                child->resize(orient,remain);
            }
            
        }
    }
    if(newSize.second<0) throw cleanAndError("Node::resize : 余量不能为负数");
}

void Node::resize_force(Orient orient,int increment){
    if(increment==0) return;
    NodeType type=getType();
    std::pair<int,int> newSize=size[orient];
    newSize.first=newSize.first+increment;
    if(type==NodeType::PART){
        newSize.second=newSize.second-increment;
        size.set(orient,newSize);
    }else if(type==NodeType::CUTLOSS || type==NodeType::LEFTOVER){
        size.set(orient,newSize);
    }else if(type==NodeType::STRUCT && orient!=next_cut_orient){
        newSize.second=newSize.second-increment;
        size.set(orient,newSize);
        for(auto& child:childs){
            child->resize_force(orient,increment);
        }
    }else if(type==NodeType::STRUCT && orient==next_cut_orient){
        newSize.second=newSize.second-increment;
        size.set(orient,newSize);
        auto dis=distribute(increment);
        int i=0;
        for(auto& child:childs){
            NodeType childType=child->getType();
            if(childType==NodeType::CUTLOSS){
                i++;
                continue;
            }else{
                child->resize_force(orient,dis[i]);
                i++;
            }
        }
    }
}

std::vector<int> Node::distribute(int num){
    int no_cutloss=(childs.size()+1)/2;
    int q=num/no_cutloss;
    int r=num%no_cutloss;
    std::vector<int> result(childs.size());
    for(int i=0;i<childs.size();i++){
        if(childs[i]->getType()==NodeType::LEFTOVER){
            result[i]=0;
        }
        else if(r>0){
            result[i]=q+1;
            r--;
        }
        else{
            result[i]=q;
        }
        
    }
    return result;
}