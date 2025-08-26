#include "../include/hyperparameters.hpp"

int CUT_LOSS=30;
double TIME_LIMIT=1000;
std::vector<int> REMAIN={50,80,100,150,200};
std::string OUTPUT_DIR="./output/";
int RANDOM_SEED=0;

size_t MAX_STAGE=8;
double UTILIZATION_RATE_LIMIT=0.95;
bool MERGE_SIZE_CHECK=false;

double AVERAGE_CUT_PUNISH=3;

int PATTERN_BATCH_SIZE=5;
int SHEET_BATCH_SIZE=5;
int SHEET_DISCARD_PROB=50;
int BLINK_PROB=10;

int DESTROY_RATE=80;
int CLOSE_SHEET_PROB=80;

int SOLUTION_GET_BEST_PROB=50;

bool VISUALIZE=true;
bool INFO_OPERATION=false;
bool INFO_HIGHS_INFO=false;
bool INFO_SELECT_RESULT=false;