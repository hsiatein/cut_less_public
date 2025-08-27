pub mod part;
pub mod sheet;

use serde::{Deserialize, Serialize};

use crate::sheet::RawSheet;


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

#[derive(Serialize, Deserialize)]
pub struct Blueprint {
    #[serde(rename = "Sheet")]
    sheet: RawSheet,
    #[serde(rename = "Node")]
    node: Node,
}

#[derive(Serialize, Deserialize)]
pub struct MetaData {
    #[serde(rename = "Sheet")]
    sheet: RawSheet,
    #[serde(rename = "Node")]
    node: Node,
}

#[derive(Debug)]
pub struct CliError(pub String);
impl warp::reject::Reject for CliError {
    
}