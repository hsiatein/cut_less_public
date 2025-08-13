#include "gtest/gtest.h"
#include <node.hpp>
#include <csp_input.hpp>

namespace test_node
{

Logger logger("test_node");
Problem problem;


TEST(Node, testNode)
{
    // logger.log_json("TestProblem",problem.to_json());
    problem=Problem::from_json("../assets/test_problem.json");
    ASSERT_EQ(problem.parts.size(),9);
    Node node0(&problem,problem.parts[0]);
    Node node1(&problem,problem.parts[1]);
    Node node2(&problem,problem.parts[2]);
    Node node3(&problem,problem.parts[3]);
    node0.addChild(node1);
    node2.addChild(node3);
    node0.addChild(node2);
    ASSERT_EQ(node0.childs[0]->parent,&node0);
    Node node4=node0;
    node4.deleteChild(node4.childs[0]);
    ASSERT_EQ(node0.childs[0]->size,node1.size);
    json j1=json::array();
    j1.push_back(node0.to_json());
    j1.push_back(node4.to_json());
    logger.log_json("ToJson",j1);
}

TEST(Node, testRotate)
{

    Node node0(&problem,problem.parts[0]);
    Node node1(&problem,problem.parts[1]);
    Node node2(&problem,problem.parts[2]);
    Node node3(&problem,problem.parts[3]);

    json j2=json::array();
    Node node5(&problem,1000,1200,1300,Orient::X);
    node5.addChild(node3);
    Node node6(&problem,1000,1200,1300,Orient::Y);
    node5.addChild(node6);
    j2.push_back(node5.to_json());
    node5.rotate(RotateOrient::X);
    j2.push_back(node5.to_json());
    logger.log_json("Rotate",j2);
}

TEST(Node, testLift)
{

    Node node0(&problem,problem.parts[0]);
    Node node1(&problem,problem.parts[1]);
    Node node2(&problem,problem.parts[2]);
    Node node3(&problem,problem.parts[3]);

    json j2=json::array();
    Node node5(&problem,1000,1200,1300,Orient::X);
    node5.addChild(node3);
    Node node6(&problem,1000,1200,1300,Orient::X);
    node6.addChild(node0);
    node6.addChild(node1);
    node5.addChild(node6);
    j2.push_back(node5.to_json());
    logger.log_json("Lift",j2);
}

TEST(Node, testCount)
{
    Node node0(&problem,problem.parts[0]);
    Node node1(&problem,problem.parts[1]);
    Node node2(&problem,problem.parts[2]);
    Node node3(&problem,problem.parts[3]);

    Node node5(&problem,1000,1200,1300,Orient::X);
    node5.addChild(node3);
    Node node6(&problem,1000,1200,1300,Orient::X);
    node6.addChild(node0);
    node6.addChild(node1);
    node5.addChild(node6);
    logger.log_json("Count",node5.to_json(),node5.count().to_json());
}

}