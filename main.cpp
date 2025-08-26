#include <solver.hpp>
#include <visualizer.hpp>
#include <argparse/argparse.hpp>


int main(int argc, char *argv[]){
    // 初始化CLI
    argparse::ArgumentParser program("cut_less", "0.1.0");
    program.add_argument("-p", "--problem").help("转换成json格式的问题").default_value("../assets/test_problem.json");
    program.add_argument("-c", "--config").help("配置文件").default_value("../config.json");
    program.parse_known_args(argc, argv);
    std::string problem_path=program.get<std::string>("problem");
    std::string config_path=program.get<std::string>("config");
    std::string output_path=program.get<std::string>("output");

    // 从json读取一个问题
    if(config_path!="") read_config(config_path);
    Problem problem=Problem::from_json(problem_path);

    // 求解
    Solver solver(&problem);
    Solution solution=solver.solve();

    // 保存解为json
    Logger logger("main");
    logger.log_json("solution",solution.to_json());

    // 可视化
    if(VISUALIZE){
        Visualizer::init();
        Visualizer::from_solution(solution);
        Visualizer::show();
    }

    return 0;
}