pub mod part;
pub mod sheet;

use serde::{Deserialize, Serialize};


#[derive(Serialize, Deserialize, Debug)]
pub struct Node {
    #[serde(rename = "Size")]
    size: Vec<u32>,
    #[serde(rename = "NodeType")]
    node_type: String,
    #[serde(rename = "Orient")]
    orient: String,
    #[serde(rename = "Childs")]
    children: Vec<Node>,
}

#[derive(Debug)]
pub struct CliError(pub String);
impl warp::reject::Reject for CliError {
    
}