#include <config.hpp>
#include <cstddef>
#include <fstream>

SolverConfig::SolverConfig(){
}

SolverConfig::SolverConfig(json config){
    this->CUT_LOSS=config["CUT_LOSS"].get<size_t>();
    this->TIME_LIMIT=config["TIME_LIMIT"].get<double>();
    std::vector<int> TEMP_REMAIN;
    for(auto& remain:config["REMAIN"]){
        TEMP_REMAIN.push_back(remain.get<int>());
    }
    this->REMAIN=TEMP_REMAIN;
    this->COEFF=config["COEFF"].get<double>();
    this->MODE=config["MODE"].get<size_t>();
    
    this->MAX_STAGE=config["MAX_STAGE"].get<size_t>();
    this->UTILIZATION_RATE_LIMIT=config["UTILIZATION_RATE_LIMIT"].get<double>();
    this->MERGE_SIZE_CHECK=config["MERGE_SIZE_CHECK"].get<bool>();
    this->REMAIN_MERGE=config["REMAIN_MERGE"].get<bool>();

    this->ENABLE_SELECTOR=config["ENABLE_SELECTOR"].get<bool>();
    this->AVERAGE_CUT_PUNISH=config["AVERAGE_CUT_PUNISH"].get<double>();
    this->HIGHS_RANDOM_SEED=config["HIGHS_RANDOM_SEED"].get<int>();

    this->LNS_RANDOM_SEED=config["LNS_RANDOM_SEED"].get<int>();
    this->PATTERN_BATCH_SIZE=config["PATTERN_BATCH_SIZE"].get<size_t>();
    this->SHEET_BATCH_SIZE=config["SHEET_BATCH_SIZE"].get<size_t>();
    this->SHEET_DISCARD_PROB=config["SHEET_DISCARD_PROB"].get<double>();
    this->BLINK_PROB=config["BLINK_PROB"].get<double>();
    this->SIZE_USAGE=config["SIZE_USAGE"].get<size_t>();

    this->DESTROY_RATE=config["DESTROY_RATE"].get<double>();
    this->CLOSE_SHEET_PROB=config["CLOSE_SHEET_PROB"].get<double>();

    this->SOLUTION_GET_BEST_PROB=config["SOLUTION_GET_BEST_PROB"].get<double>();
    this->PREFER_BENCHMARK_MODE=config["PREFER_BENCHMARK_MODE"].get<int>();

    this->VISUALIZE=config["VISUALIZE"].get<bool>();
    if(config["RUNTIME_LOG"].get<bool>()){
        auto info=config["INFO"];
        this->INFO_GENERATE_RESULT=info["GENERATE_RESULT"].get<bool>();
        this->INFO_HIGHS_INFO=info["HIGHS_INFO"].get<bool>();
        this->INFO_SELECT_RESULT=info["SELECT_RESULT"].get<bool>();
        this->INFO_OPERATION=info["OPERATION"].get<bool>();
        this->INFO_RECREATE=info["RECREATE"].get<bool>();
    }
}

SolverConfig SolverConfig::read_config(std::string path){
    std::ifstream f(path);
    json config = json::parse(f);
    return SolverConfig(config);
}
