pub mod config_v4;

use serde::{Serialize, Deserialize};


#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct SolverConfig {
    // 通用
    #[serde(default = "default_time_limit", rename = "TIME_LIMIT")]
    pub time_limit: f64,
    #[serde(default = "default_cut_loss", rename = "CUT_LOSS")]
    pub cut_loss: usize,
    #[serde(default = "default_remain", rename = "REMAIN")]
    pub remain: Vec<i32>,
    #[serde(default = "default_coeff", rename = "COEFF")]
    pub coeff: f64,

    // 组合器相关
    #[serde(default = "default_max_stage", rename = "MAX_STAGE")]
    pub max_stage: usize,
    #[serde(default = "default_utilization_rate_limit", rename = "UTILIZATION_RATE_LIMIT")]
    pub utilization_rate_limit: f64,
    #[serde(default = "default_merge_size_check", rename = "MERGE_SIZE_CHECK")]
    pub merge_size_check: bool,

    // 选择器相关
    #[serde(default = "default_average_cut_punish", rename = "AVERAGE_CUT_PUNISH")]
    pub average_cut_punish: f64,
    #[serde(default = "default_highs_random_seed", rename = "HIGHS_RANDOM_SEED")]
    pub highs_random_seed: i32,

    // LNS相关
    #[serde(default = "default_lns_random_seed", rename = "LNS_RANDOM_SEED")]
    pub lns_random_seed: i32,
    #[serde(default = "default_pattern_batch_size", rename = "PATTERN_BATCH_SIZE")]
    pub pattern_batch_size: usize,
    #[serde(default = "default_sheet_batch_size", rename = "SHEET_BATCH_SIZE")]
    pub sheet_batch_size: usize,
    #[serde(default = "default_sheet_discard_prob", rename = "SHEET_DISCARD_PROB")]
    pub sheet_discard_prob: f64,
    #[serde(default = "default_blink_prob", rename = "BLINK_PROB")]
    pub blink_prob: f64,

    // 破坏解的比例
    #[serde(default = "default_destroy_rate", rename = "DESTROY_RATE")]
    pub destroy_rate: f64,
    #[serde(default = "default_close_sheet_prob", rename = "CLOSE_SHEET_PROB")]
    pub close_sheet_prob: f64,

    // 使用最优解初始化的概率
    #[serde(default = "default_solution_get_best_prob", rename = "SOLUTION_GET_BEST_PROB")]
    pub solution_get_best_prob: f64,

    // 信息输出相关
    #[serde(default = "default_visualize", rename = "VISUALIZE")]
    pub visualize: bool,
    #[serde(default = "default_runtime_log", rename = "RUNTIME_LOG")]
    pub runtime_log: bool,
    #[serde(default, rename = "INFO")]
    pub info: InfoConfig,
}

fn default_time_limit() -> f64 { 1000.0 }
fn default_cut_loss() -> usize { 30 }
fn default_remain() -> Vec<i32> { vec![50, 80, 100, 150, 200] }
fn default_coeff() -> f64 { crate::FACTOR }

fn default_max_stage() -> usize { 8 }
fn default_utilization_rate_limit() -> f64 { 0.95 }
fn default_merge_size_check() -> bool { false }

fn default_average_cut_punish() -> f64 { 3.0 }
fn default_highs_random_seed() -> i32 { 0 }

fn default_lns_random_seed() -> i32 { 0 }
fn default_pattern_batch_size() -> usize { 5 }
fn default_sheet_batch_size() -> usize { 5 }
fn default_sheet_discard_prob() -> f64 { 0.5 }
fn default_blink_prob() -> f64 { 0.1 }

fn default_destroy_rate() -> f64 { 0.8 }
fn default_close_sheet_prob() -> f64 { 0.8 }

fn default_solution_get_best_prob() -> f64 { 0.5 }
fn default_visualize() -> bool { false }
fn default_runtime_log() -> bool { true }

impl Default for SolverConfig {
    fn default() -> Self {
        Self {
            time_limit: 1000.0,
            cut_loss: 30,
            remain: vec![50, 80, 100, 150, 200],
            coeff: default_coeff(),

            max_stage: 8,
            utilization_rate_limit: 0.95,
            merge_size_check: false,

            average_cut_punish: 3.0,
            highs_random_seed: 0,

            lns_random_seed: 0,
            pattern_batch_size: 5,
            sheet_batch_size: 5,
            sheet_discard_prob: 0.5,
            blink_prob: 0.1,

            destroy_rate: 0.8,
            close_sheet_prob: 0.8,

            solution_get_best_prob: 0.5,

            visualize: false,
            runtime_log: true,
            info: InfoConfig::default(),
        }
    }
}

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct InfoConfig {
    #[serde(default = "default_generate_result",rename = "GENERATE_RESULT")]
    pub generate_result: bool,
    #[serde(default = "default_highs_info",rename = "HIGHS_INFO")]
    pub highs_info: bool,
    #[serde(default = "default_select_result",rename = "SELECT_RESULT")]
    pub select_result: bool,
    #[serde(default = "default_operation",rename = "OPERATION")]
    pub operation: bool,
}

fn default_generate_result() -> bool { false }
fn default_highs_info() -> bool { false }
fn default_select_result() -> bool { false }
fn default_operation() -> bool { false }

impl Default for InfoConfig {
    fn default() -> Self {
        Self {
            generate_result: false,
            highs_info: false,
            select_result: false,
            operation: false,
        }
    }
}

pub trait AsConfig {
    fn to_config(&self)->SolverConfig;    
}