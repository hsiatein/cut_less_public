pub mod part;
pub mod sheet;
pub mod config;
pub mod request_data;

use serde::{Deserialize, Serialize};
use crate::sheet::raw_sheet_v3::RawSheetV3;


#[derive(Serialize, Deserialize, Debug)]
pub struct Node {
    #[serde(rename = "Size")]
    size: Vec<usize>,
    #[serde(rename = "NodeType")]
    node_type: String,
    #[serde(rename = "Orient")]
    orient: String,
    #[serde(rename = "Childs")]
    children: Vec<Node>,
}

#[derive(Serialize, Deserialize)]
pub struct Blueprint {
    #[serde(rename = "Sheet")]
    sheet: RawSheetV3,
    #[serde(rename = "Node")]
    node: Node,
}

#[derive(Serialize, Deserialize)]
pub struct MetaData {
    #[serde(rename = "CutCost")]
    cut_cost: usize,
    #[serde(rename = "VolumeCost")]
    node: f64,
}



#[derive(Debug)]
pub struct CliError(pub String);
impl warp::reject::Reject for CliError {
    
}
