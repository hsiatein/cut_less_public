#include "../include/hyperparameters.hpp"

// 1 -> 0.1mm
int CUT_LOSS=30;
int TIME_LIMIT=1000;
std::vector<int> REMAIN={50,80,100,150,200};
size_t MAX_STAGE=8;
double UTILIZATION_RATE_LIMIT=0.95;
std::string OUTPUT_DIR="./output/";
int PATTERN_BATCH_SIZE=5;
int RANDOM_SEED=0;
int SHEET_BATCH_SIZE=5;
int SHEET_DISCARD_PROB=50;
int LNS_TIME=1000;
int DESTROY_RATE=80;
int BLINK_PROB=10;
int SOLUTION_GET_BEST_PROB=50;
int CLOSE_SHEET_PROB=80;
