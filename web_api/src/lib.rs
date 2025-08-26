use serde::{Deserialize, Serialize};

#[derive(Serialize,Deserialize)]
pub struct Part{
    #[serde(rename = "ID")]
    id:u32,
    #[serde(rename = "Size")]
    size:Vec<u32>,
    #[serde(rename = "Rotatable")]
    rotatable:bool,
    #[serde(rename = "qty")]
    qty:u32
}

#[derive(Serialize,Deserialize)]
pub struct Sheet{
    #[serde(rename = "ID")]
    id:u32,
    #[serde(rename = "Size")]
    size:Vec<u32>,
    #[serde(rename = "Small")]
    small:bool,
    #[serde(rename = "qty")]
    qty:u32
}

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