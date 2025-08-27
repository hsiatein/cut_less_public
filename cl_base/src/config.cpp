#include <config.hpp>

std::vector<int> REMAIN={50,80,100,150,200};
std::string OUTPUT_DIR="./output/";

bool VISUALIZE=true;
bool INFO_GENERATE_RESULT=false;
bool INFO_HIGHS_INFO=false;
bool INFO_SELECT_RESULT=false;
bool INFO_OPERATION=false;

SolverConfig::SolverConfig(){
}

SolverConfig::SolverConfig(json config){
    this->CUT_LOSS=config["CUT_LOSS"].get<size_t>();
    this->TIME_LIMIT=config["TIME_LIMIT"].get<double>();
    
    this->MAX_STAGE=config["MAX_STAGE"].get<size_t>();
    this->UTILIZATION_RATE_LIMIT=config["UTILIZATION_RATE_LIMIT"].get<double>();
    this->MERGE_SIZE_CHECK=config["MERGE_SIZE_CHECK"].get<bool>();

    this->AVERAGE_CUT_PUNISH=config["AVERAGE_CUT_PUNISH"].get<double>();
    this->HIGHS_RANDOM_SEED=config["HIGHS_RANDOM_SEED"].get<int>();

    this->LNS_RANDOM_SEED=config["LNS_RANDOM_SEED"].get<int>();
    this->PATTERN_BATCH_SIZE=config["PATTERN_BATCH_SIZE"].get<size_t>();
    this->SHEET_BATCH_SIZE=config["SHEET_BATCH_SIZE"].get<size_t>();
    this->SHEET_DISCARD_PROB=config["SHEET_DISCARD_PROB"].get<double>();
    this->BLINK_PROB=config["BLINK_PROB"].get<double>();

    this->DESTROY_RATE=config["DESTROY_RATE"].get<double>();
    this->CLOSE_SHEET_PROB=config["CLOSE_SHEET_PROB"].get<double>();

    this->SOLUTION_GET_BEST_PROB=config["SOLUTION_GET_BEST_PROB"].get<double>();
}