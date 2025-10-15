#include "config.hpp"
#include "solution.hpp"
#include <solver.hpp>
#include <argparse/argparse.hpp>


int main(int argc, char *argv[]){
    // 初始化CLI
    argparse::ArgumentParser program("cut_less", "0.1.0");
    program.add_argument("-p", "--problem").help("转换成json格式的问题").default_value("");
    program.add_argument("-c", "--config").help("配置文件").default_value("");
    program.add_argument("-o", "--output").help("输出").default_value("./output");
    program.add_argument("-n", "--name").help("名字").default_value("solution");
    program.parse_known_args(argc, argv);
    std::string problem_string=program.get<std::string>("problem");
    std::string config_string=program.get<std::string>("config");
    std::string output_path=program.get<std::string>("output");
    std::string name=program.get<std::string>("name");

    // 从json读取一个问题
    SolverConfig config(json::parse(config_string));
    json problem_json=json::parse(problem_string);
    Problem problem(problem_json);

    // 求解
    Solver solver(&problem,config);
    Solution solution=solver.solve();
    Metadata metadata(&solution);

    // 保存解为json
    Logger logger("main",output_path);
    logger.log_json(name,solution.to_json());
    logger.log_json(name+"_metadata",metadata.to_json());

    return 0;
}