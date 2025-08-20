#pragma once
#include <vector>
#include "hyperparameters.hpp"
#include "utils.hpp"
#include "problem.hpp"



struct cl_base_EXPORT Node{
    Problem* problem;
    Size size;
    size_t partTypeID;
    std::vector<Node*> childs;
    Node* parent;
    Orient next_cut_orient;

    Node();
    Node(Problem* problem,int width,int height,int thick,size_t partTypeID,Node* parent,Orient next_cut_orient);
    Node(Problem* problem,int width,int height,int thick,Orient next_cut_orient);
    Node(Problem* problem,int width,int height,int thick,Orient next_cut_orient,const Vec3i& remain);
    Node(Problem* problem,const Size& size,int partTypeID,Orient next_cut_orient);
    Node(Problem* problem,const PartType& partType);
    Node(Problem* problem,const PartType& partType,const Vec3i& remain);
    Node(const Node& node);
    Node(Node&& other);
    ~Node();
    /// @brief 添加子节点
    /// @param child 
    void addChild(Node* child);
    void addChild(int width,int height,int thick,size_t partTypeID,Orient next_cut_orient);
    void addChild(PartType partType);
    void addChild(const Node& node);
    bool containChild(const Node* child);
    void deleteChild(Node* child);
    void liftChild(Node* child);
    void resize(Orient orient,int increment);
    void resize_force(Orient orient,int increment);
    std::vector<int> distribute(int num);
    NodeType getType() const;
    PartsNum count();
    // only rotate itself
    void rotate(RotateOrient rotateOrient);
    json to_json() const;
};