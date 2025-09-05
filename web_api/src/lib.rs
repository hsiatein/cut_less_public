pub mod part;
pub mod sheet;
pub mod config;
pub mod request_data;
pub mod response;
pub mod handler;

use serde::{Deserialize, Serialize};

static factor:f64=10000.;


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
