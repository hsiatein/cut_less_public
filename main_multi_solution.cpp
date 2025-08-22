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

    // 从json读取一个问题
    if(config_path!="") read_config(config_path);
    Problem problem=Problem::from_json(problem_path);

    // 求解 & 计时
    auto start = std::chrono::high_resolution_clock::now();

    Solver solver(&problem);
    auto solutions=solver.solve_multi_solution();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << std::fixed <<"\033[33m"<< "[Execution time: " << duration.count() << " ms]\033[0m" << std::endl;

    // 保存解为json
    for(int i=0;i<solutions.size();i++){
        Logger logger("main");
        logger.log_json("solution"+std::to_string(i+1),solutions[i].to_json());
    }

    // 可视化
    Visualizer::init();
    Visualizer::from_solutions(&solutions);
    Visualizer::show();

    return 0;
}