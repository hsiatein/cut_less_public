#include <solver.hpp>
#include <visualizer.hpp>
#include <argparse/argparse.hpp>


int main(int argc, char *argv[]){
    // 初始化CLI
    argparse::ArgumentParser program("cut_less", "0.1.0");
    program.add_argument("-p", "--problem").help("转换成json格式的问题").default_value("");
    program.add_argument("-c", "--config").help("配置文件").default_value("../config.json");
    program.add_argument("-o", "--output").help("输出").default_value("");
    program.parse_known_args(argc, argv);
    std::string problem_string=program.get<std::string>("problem");
    std::string config_path=program.get<std::string>("config");
    std::string output_path=program.get<std::string>("output");

    // 从json读取一个问题
    if(output_path!="") OUTPUT_DIR=output_path;
    if(config_path!="") read_config(config_path);
    json problem_json=json::parse(problem_string);
    Problem problem(problem_json);

    // 求解
    Solver solver(&problem);
    Solution solution=solver.solve();

    // 保存解为json
    Logger logger("main");
    logger.log_json("solution",solution.to_json());

    return 0;
}