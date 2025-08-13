#pragma once
#include <pattern.hpp>


struct Cube{
public:
    Vec3i location;
    Vec3i size;
    NodeType type;
    Orient orient;
    std::vector<Cube*> childs;
    Cube(const Node* node,Vec3i location);
    Cube(const Cube& cube);
    ~Cube();
    std::vector<Cube*> get_all_cubes();
private:
    
};