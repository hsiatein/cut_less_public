#pragma once
#include <vector>
#include <string>

// 1 -> 0.1mm
extern int CUT_LOSS;
extern double TIME_LIMIT;
extern std::vector<int> REMAIN;
extern std::string OUTPUT_DIR;
extern int RANDOM_SEED;

extern size_t MAX_STAGE;
extern double UTILIZATION_RATE_LIMIT;
extern bool MERGE_SIZE_CHECK;

extern double AVERAGE_CUT_PUNISH;

extern int PATTERN_BATCH_SIZE;
extern int SHEET_BATCH_SIZE;
extern int SHEET_DISCARD_PROB;
extern int BLINK_PROB;

extern int DESTROY_RATE;
extern int CLOSE_SHEET_PROB;

extern int SOLUTION_GET_BEST_PROB;

extern bool VISUALIZE;
extern bool INFO_OPERATION;
extern bool INFO_HIGHS_INFO;
extern bool INFO_SELECT_RESULT;