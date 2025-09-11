use serde::{Deserialize, Serialize};
use crate::request_data::ToProblem;
use crate::config::config_v4::SolverConfigV4;
use crate::sheet::raw_sheet_v5::RawSheetsV5;
use crate::part::raw_part_v5::RawPartsV5;
use crate::request_data::request_data_v4::RequestDataV4;

#[derive(Serialize,Deserialize)]
pub struct RequestDataV5 {
    #[serde(default, rename = "Config")]
    pub config: SolverConfigV4,
    #[serde(flatten)]
    pub parts: RawPartsV5,
    #[serde(flatten)]
    pub sheets: RawSheetsV5,
}

impl RequestDataV5{
    pub fn split(&self)->Vec<RequestDataV5> {
        let parts=self.parts.regularize();
        let part_groups = parts.to_groups();
        let groups=self.sheets.to_groups(part_groups);
        groups.into_iter().map(|(x,y)| RequestDataV5{
            config:self.config.clone(),parts:x,sheets:y
        }).collect()
    }

    pub fn to_request_data_v4(&self)->RequestDataV4 {
        RequestDataV4{
            config:self.config.clone(),
            parts:self.parts.to_parts_v4(),
            sheets:self.sheets.to_sheets_v4()
        }
    }
}

impl ToProblem for RequestDataV5{
    fn to_problem(&self)->super::Problem {
        self.to_request_data_v4().to_problem()
    }
}