#include <solver.hpp>
#include <visualizer.hpp>



int main(int argc, char** argv) {
    // 从json读取一个问题
    SolverConfig config;
    config=read_config("../config.json");
    Problem problem=Problem::from_json("../assets/test_problem.json");

    // 求解
    Solver solver(&problem,config);
    Solution solution=solver.solve();

    // 可视化
    if(VISUALIZE){
        Visualizer::init();
        Visualizer::from_solution(solution);
        Visualizer::show();
    }

    return 0;
}