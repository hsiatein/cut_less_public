#include "../include/hyperparameters.hpp"

// 1 -> 0.1mm
cl_base_EXPORT int CUT_LOSS=30;
cl_base_EXPORT int TIME_LIMIT=1000;
cl_base_EXPORT std::vector<int> REMAIN={50,80,100,150,200};
cl_base_EXPORT size_t MAX_STAGE=8;
cl_base_EXPORT double UTILIZATION_RATE_LIMIT=0.95;
cl_base_EXPORT std::string OUTPUT_DIR="./output/";
cl_base_EXPORT int PATTERN_BATCH_SIZE=5;
cl_base_EXPORT int RANDOM_SEED=0;
cl_base_EXPORT int SHEET_BATCH_SIZE=5;
cl_base_EXPORT int SHEET_DISCARD_PROB=50;
cl_base_EXPORT int LNS_TIME=1000;
cl_base_EXPORT int DESTROY_RATE=80;
cl_base_EXPORT int BLINK_PROB=10;
cl_base_EXPORT int SOLUTION_GET_BEST_PROB=50;
cl_base_EXPORT int CLOSE_SHEET_PROB=80;
