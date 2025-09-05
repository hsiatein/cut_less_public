pub mod request_data_v3;

use serde::{Deserialize, Serialize};
use crate::sheet::Sheet;
use crate::part::Part;
use crate::config::SolverConfig;

#[derive(Serialize,Deserialize)]
pub struct RequestData {
    #[serde(default, rename = "Config")]
    pub config: SolverConfig,
    #[serde(rename = "Parts")]
    pub parts: Vec<Part>,
    #[serde(rename = "Sheets")]
    pub sheets: Vec<Sheet>,
}

#[derive(Serialize,Deserialize)]
pub struct Problem {
    #[serde(rename = "Parts")]
    pub parts: Vec<Part>,
    #[serde(rename = "Sheets")]
    pub sheets: Vec<Sheet>,
}

pub trait ToProblem{
    fn to_problem(&self)->Problem;
}