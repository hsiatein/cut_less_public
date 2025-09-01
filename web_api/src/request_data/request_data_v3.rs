use serde::{Deserialize, Serialize};
use crate::request_data::ToProblem;
use crate::sheet::{AsSheets};
use crate::part::{AsParts};
use crate::config::SolverConfig;
use crate::sheet::raw_sheet_v3::RawSheetsV3;
use crate::part::raw_part_v3::RawPartsV3;

#[derive(Serialize,Deserialize)]
pub struct RequestDataV3 {
    #[serde(default, rename = "Config")]
    pub config: SolverConfig,
    #[serde(rename = "Parts")]
    pub parts: RawPartsV3,
    #[serde(rename = "Sheets")]
    pub sheets: RawSheetsV3,
}

impl ToProblem for RequestDataV3{
    fn to_problem(&self)->super::Problem {
        super::Problem{parts:self.parts.to_parts(),sheets:self.sheets.to_sheets()}
    }
}