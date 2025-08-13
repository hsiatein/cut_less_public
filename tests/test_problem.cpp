#include "gtest/gtest.h"
#include <problem.hpp>

namespace test_problem{

Logger logger("test_problem");
// Pattern pattern1(nullptr,100,200,250,Orient::X);
// Pattern pattern2(nullptr,150,220,430,Orient::Y);
// Pattern pattern3(nullptr,150,220,430,Orient::Y);
// Pattern pattern4(nullptr,150,220,430,Orient::Z);

TEST(Probelm,testFromJson){
    Problem problem=Problem::from_json("../assets/test_problem.json");
    logger.log("1");
    json j=problem.to_json();
    logger.log("2");
    logger.log_json("FromJson",j);
}

TEST(Probelm,testSort){
    Problem problem=Problem::from_json("../assets/test_problem.json");
    logger.log("1");
    json j=problem.to_json();
    logger.log("2");
    logger.log_json("SortedJson",j);
}
}