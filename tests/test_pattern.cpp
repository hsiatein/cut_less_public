#include "gtest/gtest.h"
#include <pattern.hpp>
#include <visualizer.hpp>

namespace test_pattern{

Logger logger("test_pattern");
Problem problem;

TEST(Pattern,testMatch){
    problem=Problem::from_json("../assets/test_problem.json");
    Pattern pattern1(&problem,problem.parts[1]);
    Pattern pattern8(&problem,problem.parts[8]);
    ASSERT_EQ(pattern1.match(pattern8,Orient::X,Orient::X),std::nullopt);
    ASSERT_EQ(pattern1.match(pattern8,Orient::X,Orient::Y),std::nullopt);
    ASSERT_EQ(pattern1.match(pattern8,Orient::X,Orient::Z),std::nullopt);
    ASSERT_EQ(pattern1.match(pattern8,Orient::Y,Orient::X),std::nullopt);
    ASSERT_EQ(pattern1.match(pattern8,Orient::Y,Orient::Y),std::nullopt);
    ASSERT_EQ(pattern1.match(pattern8,Orient::Y,Orient::Z),std::nullopt);
    ASSERT_EQ(pattern1.match(pattern8,Orient::Z,Orient::X),std::nullopt);
    ASSERT_EQ(pattern1.match(pattern8,Orient::Z,Orient::Y),std::nullopt);
    ASSERT_EQ(pattern1.match(pattern8,Orient::Z,Orient::Z),1050);
}

TEST(Pattern,testMatch1D){
    problem=Problem::from_json("../assets/test_problem.json");
    Pattern pattern1(&problem,problem.parts[1]);
    Pattern pattern8(&problem,problem.parts[8]);
    auto match=pattern1.collect_match_1D(pattern8);
    ASSERT_EQ(match.size(),1);
    ASSERT_EQ(match[0],OrientMatch(Orient::Z,Orient::Z,1050));
    Pattern pattern2(pattern1,RotateOrient::Y);
    Pattern pattern9(pattern8,RotateOrient::X);
    match=pattern2.collect_match_1D(pattern9);
    ASSERT_EQ(match.size(),1);
    ASSERT_EQ(match[0],OrientMatch(Orient::X,Orient::Y,1050));
}

TEST(Pattern,testResize){
    problem=Problem::from_json("../assets/test_problem.json");
    Pattern pattern1(&problem,problem.parts[1]);
    Pattern pattern8(&problem,problem.parts[8]);
    auto match=pattern1.collect_match_1D(pattern8);
    Pattern pattern1b(pattern1,RotateOrient::Y);
    Pattern pattern8b(pattern8,RotateOrient::Y);
    match=pattern1b.collect_match_1D(pattern8b);
    ASSERT_EQ(match.size(),1);
    ASSERT_EQ(match[0],OrientMatch(Orient::X,Orient::X,1050));
    Pattern pattern1c(pattern1b);
    Pattern pattern8c(pattern8b);
    pattern1c.resize(Orient::X,1050);
    pattern8c.resize(Orient::X,1050);
    logger.log_json("testResize",pattern1b.top->to_json(),pattern1c.top->to_json(),pattern8b.top->to_json(),pattern8c.top->to_json());
}

TEST(Pattern,testResizeMerge){
    problem=Problem::from_json("../assets/test_problem.json");
    Pattern pattern1(&problem,problem.parts[1]);
    Pattern pattern8(&problem,problem.parts[8]);
    auto match=pattern1.collect_match_1D(pattern8);
    Pattern pattern1b(pattern1,RotateOrient::Y);
    Pattern pattern8b(pattern8,RotateOrient::Y);
    match=pattern1b.collect_match_1D(pattern8b);
    ASSERT_EQ(match.size(),1);
    ASSERT_EQ(match[0],OrientMatch(Orient::X,Orient::X,1050));
    Pattern pattern1c(pattern1b);
    Pattern pattern8c(pattern8b);
    pattern1c.resize(Orient::X,1050);
    pattern8c.resize(Orient::X,1050);
    pattern8c.resize_or_merge(Orient::Z,1900);
    logger.log_json("testResizeMerge",pattern1b.to_json(),pattern1c.to_json(),pattern8b.to_json(),pattern8c.to_json());
}

// TEST(Pattern,test_generate_merged_pattern){
//     problem=Problem::from_json("../assets/test_problem.json");
//     Pattern pattern1(&problem,problem.parts[1]);
//     Pattern pattern8(&problem,problem.parts[8]);
//     auto patterns=Pattern::generate_merged_pattern(pattern1,pattern8);
//     json result=json::array();
//     for(const auto& pattern:patterns){
//         result.push_back(pattern.to_json());
//     }
//     logger.log_json("test_generate_merged_pattern",result);
//     Visualizer::init();
//     for(const auto& pattern:patterns){
//         Cube cube(pattern.top,{0,0,0});
//         Visualizer::layouts.push_back(cube);
//     }
    
//     Visualizer::show();
// }

}





