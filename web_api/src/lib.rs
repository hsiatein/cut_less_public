pub mod part;
pub mod sheet;
pub mod config;
pub mod request_data;
pub mod response;

use serde::{Deserialize, Serialize};


#[derive(Serialize, Deserialize)]
pub struct MetaData {
    #[serde(rename = "CutCost")]
    cut_cost: usize,
    #[serde(rename = "VolumeCost")]
    volume_cost: f64,
}



#[derive(Debug)]
pub struct CliError(pub String);
impl warp::reject::Reject for CliError {
    
}
