#pragma once
#include <vector>
#include <string>

// 1 -> 0.1mm
extern int CUT_LOSS;
extern int TIME_LIMIT;
extern std::vector<int> REMAIN;
extern size_t MAX_STAGE;
extern double UTILIZATION_RATE_LIMIT;
extern std::string OUTPUT_DIR;
extern int PATTERN_BATCH_SIZE;
extern int RANDOM_SEED;
extern int SHEET_BATCH_SIZE;
extern int SHEET_DISCARD_PROB;
extern int LNS_TIME;
extern int DESTROY_RATE;
extern int BLINK_PROB;
extern int SOLUTION_GET_BEST_PROB;
extern int CLOSE_SHEET_PROB;
extern double AVERAGE_CUT_PUNISH;
extern bool VISUALIZE;

extern bool INFO_OPERATION;
extern bool INFO_HIGHS_INFO;
extern bool INFO_SELECT_RESULT;