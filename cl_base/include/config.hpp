#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

struct SolverConfig{
    // 通用

    double TIME_LIMIT=1000;
    size_t CUT_LOSS=30;
    std::vector<int> REMAIN={50,80,100,150,200};
    double COEFF=10;
    size_t MODE=0;

    // 组合器相关

    size_t MAX_STAGE=8;
    double UTILIZATION_RATE_LIMIT=0.95;
    bool MERGE_SIZE_CHECK=false;
    bool REMAIN_MERGE=false;

    // 选择器相关

    bool ENABLE_SELECTOR=true;
    double AVERAGE_CUT_PUNISH=3;
    int HIGHS_RANDOM_SEED=0;

    // LNS相关

    int LNS_RANDOM_SEED=0;
    // 一批选择多少个pattern生成插入选项
    size_t PATTERN_BATCH_SIZE=5;
    // 没有选项时，打开多少个原料
    size_t SHEET_BATCH_SIZE=5;
    // 打开符合要求原料的概率（数值越高，打开的原料种类越固定）
    double SHEET_DISCARD_PROB=0.5;
    // 不按启发函数选择插入选项的概率（纯随机选择）
    double BLINK_PROB=0.1;

    // 破坏解的比例
    double DESTROY_RATE=0.8;
    // 没有东西的原料被关闭的概率
    double CLOSE_SHEET_PROB=0.8;

    // 使用最优解初始化的概率
    double SOLUTION_GET_BEST_PROB=0.5;
    // "VOLUME","CUT",
    size_t PREFER_BENCHMARK_MODE=0;

    // 信息输出相关
    bool VISUALIZE=false;
    bool INFO_GENERATE_RESULT=false;
    bool INFO_HIGHS_INFO=false;
    bool INFO_SELECT_RESULT=false;
    bool INFO_OPERATION=false;
    bool INFO_RECREATE=false;

    SolverConfig();
    SolverConfig(json config);
    inline std::string general_to_string(){
        json j=json::array();
        for(auto a:REMAIN){
            j.push_back(((double)a)/COEFF);
        }
        std::string result="<General>\nTIME_LIMIT: "+std::to_string(TIME_LIMIT)+", CUT_LOSS: "+std::to_string(CUT_LOSS/COEFF)+", REDUNDANCIES="+j.dump();
        return result;
    }
    inline std::string merger_to_string(){
        std::string result="<Merger>\nMAX_STAGE: "+std::to_string(MAX_STAGE)+", UTILIZATION_RATE_LIMIT: "+std::to_string(UTILIZATION_RATE_LIMIT)+", MERGE_SIZE_CHECK: "+(MERGE_SIZE_CHECK?"true":"false");
        return result;
    }
    inline std::string selector_to_string(){
        std::string result="<Selector>\nAVERAGE_CUT_PUNISH: "+std::to_string(AVERAGE_CUT_PUNISH)+", HIGHS_RANDOM_SEED: "+std::to_string(HIGHS_RANDOM_SEED);
        return result;
    }
    inline std::string lns_to_string(){
        std::string result="<LNS>\nLNS_RANDOM_SEED: "+std::to_string(LNS_RANDOM_SEED)+", PATTERN_BATCH_SIZE: "+std::to_string(PATTERN_BATCH_SIZE)+", SHEET_BATCH_SIZE: "+std::to_string(SHEET_BATCH_SIZE)+", SHEET_DISCARD_PROB: "+std::to_string(SHEET_DISCARD_PROB)+"\nBLINK_PROB: "+std::to_string(BLINK_PROB)+", DESTROY_RATE: "+std::to_string(DESTROY_RATE)+", CLOSE_SHEET_PROB: "+std::to_string(CLOSE_SHEET_PROB)+", SOLUTION_GET_BEST_PROB: "+std::to_string(SOLUTION_GET_BEST_PROB);
        return result;
    }
    inline std::string to_string(){
        std::string result=general_to_string()+"\n"+merger_to_string()+"\n"+selector_to_string()+"\n"+lns_to_string()+"\n";
        return result;
    }
    static SolverConfig read_config(std::string path);
};