use serde::{Serialize, Deserialize};

use crate::config::{AsConfig, SolverConfig};


#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct SolverConfigV4 {
    // 通用
    #[serde(default = "super::default_time_limit", rename = "TIME_LIMIT")]
    pub time_limit: f64,
    #[serde(default = "default_cut_loss", rename = "CUT_LOSS")]
    pub cut_loss: f64,
    #[serde(default = "default_remain", rename = "REMAIN")]
    pub remain: Vec<f64>,

    // 组合器相关
    #[serde(default = "super::default_max_stage", rename = "MAX_STAGE")]
    pub max_stage: usize,
    #[serde(default = "super::default_utilization_rate_limit", rename = "UTILIZATION_RATE_LIMIT")]
    pub utilization_rate_limit: f64,
    #[serde(default = "super::default_merge_size_check", rename = "MERGE_SIZE_CHECK")]
    pub merge_size_check: bool,
    #[serde(default = "super::default_remain_merge", rename = "REMAIN_MERGE")]
    pub remain_merge: bool,

    // 选择器相关
    #[serde(default = "super::default_average_cut_punish", rename = "AVERAGE_CUT_PUNISH")]
    pub average_cut_punish: f64,
    #[serde(default = "super::default_highs_random_seed", rename = "HIGHS_RANDOM_SEED")]
    pub highs_random_seed: i32,

    // LNS相关
    #[serde(default = "super::default_lns_random_seed", rename = "LNS_RANDOM_SEED")]
    pub lns_random_seed: i32,
    #[serde(default = "super::default_pattern_batch_size", rename = "PATTERN_BATCH_SIZE")]
    pub pattern_batch_size: usize,
    #[serde(default = "super::default_sheet_batch_size", rename = "SHEET_BATCH_SIZE")]
    pub sheet_batch_size: usize,
    #[serde(default = "super::default_sheet_discard_prob", rename = "SHEET_DISCARD_PROB")]
    pub sheet_discard_prob: f64,
    #[serde(default = "super::default_blink_prob", rename = "BLINK_PROB")]
    pub blink_prob: f64,

    // 破坏解的比例
    #[serde(default = "super::default_destroy_rate", rename = "DESTROY_RATE")]
    pub destroy_rate: f64,
    #[serde(default = "super::default_close_sheet_prob", rename = "CLOSE_SHEET_PROB")]
    pub close_sheet_prob: f64,

    // 使用最优解初始化的概率
    #[serde(default = "super::default_solution_get_best_prob", rename = "SOLUTION_GET_BEST_PROB")]
    pub solution_get_best_prob: f64,

    // 信息输出相关
    #[serde(default = "super::default_visualize", rename = "VISUALIZE")]
    pub visualize: bool,
    #[serde(default = "super::default_runtime_log", rename = "RUNTIME_LOG")]
    pub runtime_log: bool,
    #[serde(default, rename = "INFO")]
    pub info: super::InfoConfig,
}

fn default_cut_loss() -> f64 { 3.0 }
fn default_remain() -> Vec<f64> { vec![5.0, 8.0, 10.0, 15.0, 20.0] }

impl Default for SolverConfigV4 {
    fn default() -> Self {
        Self {
            time_limit: 1000.0,
            cut_loss: 3.,
            remain: vec![5., 8., 10., 15., 20.],

            max_stage: 8,
            utilization_rate_limit: 0.95,
            merge_size_check: false,
            remain_merge: false,

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
            info: super::InfoConfig::default(),
        }
    }
}

impl AsConfig for SolverConfigV4 {
    fn to_config(&self)->SolverConfig{
        SolverConfig{
            time_limit:self.time_limit,
            cut_loss:(self.cut_loss*crate::FACTOR).round() as usize,
            remain:self.remain.iter().map(|remain|(remain*crate::FACTOR).round() as i32).collect(),
            coeff:super::default_coeff(),
            max_stage:self.max_stage,
            utilization_rate_limit:self.utilization_rate_limit,
            merge_size_check:self.merge_size_check,
            remain_merge:self.remain_merge,
            average_cut_punish:self.average_cut_punish,
            highs_random_seed:self.highs_random_seed,
            lns_random_seed:self.lns_random_seed,
            pattern_batch_size:self.pattern_batch_size,
            sheet_batch_size:self.sheet_batch_size,
            sheet_discard_prob:self.sheet_discard_prob,
            blink_prob:self.blink_prob,
            destroy_rate:self.destroy_rate,
            close_sheet_prob:self.close_sheet_prob,
            solution_get_best_prob:self.solution_get_best_prob,
            visualize:self.visualize,
            runtime_log:self.runtime_log,
            info:self.info.clone()
        }
    }
}

