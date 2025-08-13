#include "../include/cube.hpp"

Cube::Cube(const Node* node,Vec3i location):location(location),size({node->size.size[0],node->size.size[2],node->size.size[1]}),type(node->getType()),orient(node->next_cut_orient){
    Vec3i childLocation=location;
    for(const Node* child:node->childs){
        Cube* cubeChild=new Cube(child,childLocation);
        childs.push_back(cubeChild);
        switch (orient)
        {
        case Orient::X:
            childLocation[0]=childLocation[0]+cubeChild->size[0];
            break;
        case Orient::Y:
            childLocation[2]=childLocation[2]+cubeChild->size[2];
            break;
        case Orient::Z:
            childLocation[1]=childLocation[1]+cubeChild->size[1];
            break;
        default:
            break;
        }
    }
}

Cube::Cube(const Cube& cube):location(cube.location),size(cube.size),type(cube.type),orient(cube.orient){
    for(auto child:cube.childs){
        childs.push_back(new Cube(*child));
    }
}


Cube::~Cube(){
    for(Cube* cube:childs){
        delete cube;
    }
}

std::vector<Cube*> Cube::get_all_cubes(){
    std::vector<Cube*> result;
    std::deque<Cube*> Q;
    Q.push_back(this);
    while (!Q.empty())
    {
        Cube* u=Q.back();
        Q.pop_back();
        for(int i=u->childs.size()-1;i>=0;i--){
            auto child=u->childs[i];
            if(child->type!=NodeType::STRUCT) result.push_back(child);
            Q.push_back(child);
        }
    }
    return result;
}

