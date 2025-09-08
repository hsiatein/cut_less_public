use serde::{Deserialize, Serialize};
use crate::config::AsConfig;
use crate::request_data::ToProblem;
use crate::sheet::{AsSheets};
use crate::part::{AsParts};
use crate::config::config_v4::SolverConfigV4;
use crate::sheet::raw_sheet_v4::RawSheetsV4;
use crate::part::raw_part_v4::RawPartsV4;
use crate::request_data::request_data_v3::RequestDataV3;

#[derive(Serialize,Deserialize)]
pub struct RequestDataV4 {
    #[serde(default, rename = "Config")]
    pub config: SolverConfigV4,
    #[serde(flatten)]
    pub parts: RawPartsV4,
    #[serde(flatten)]
    pub sheets: RawSheetsV4,
}

impl RequestDataV4{
    pub fn to_request_data_v3(&self)->RequestDataV3 {
        RequestDataV3{
            config:self.config.to_config(),
            parts:self.parts.to_parts_v3(),
            sheets:self.sheets.to_sheets_v3()
        }
    }
}

impl ToProblem for RequestDataV4{
    fn to_problem(&self)->super::Problem {
        self.to_request_data_v3().to_problem()
    }
}