#include "gtest/gtest.h"
#include <solver.hpp>
// #include <visualizer.hpp>

namespace test_solver{

Logger logger("test_solver");
Problem problem;

TEST(Solver,testSolver){
    // 从json读取一个问题
    read_config("../config.json");
    problem=Problem::from_json("../assets/test_problem2.json");
        
    // 求解
    Solver solver(&problem);
    Solution solution=solver.solve();

    // 保存解为json
    Logger logger("main");
    logger.log_json("solution",solution.to_json());

    // // 可视化
    // Visualizer::init();
    // Visualizer::from_solution(solution);
    // Visualizer::show();

}


}