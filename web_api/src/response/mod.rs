pub mod response_v3;

use serde::{Deserialize, Serialize};
use crate::sheet::Sheet;

#[derive(Serialize, Deserialize, Debug)]
pub struct Node {
    #[serde(rename = "Size")]
    size: Vec<i32>,
    #[serde(rename = "NodeType")]
    node_type: String,
    #[serde(rename = "Orient")]
    orient: String,
    #[serde(rename = "Childs")]
    children: Vec<Node>,
}


#[derive(Serialize, Deserialize)]
pub struct Blueprint{
    #[serde(rename = "Sheet")]
    pub sheet: Sheet,
    #[serde(rename = "Root")]
    pub root: Node,
}

#[derive(Serialize,Deserialize)]
pub struct Response{
    pub solution: Vec<Blueprint>,
}
