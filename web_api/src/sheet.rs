use serde::{Deserialize, Serialize};

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

#[derive(Serialize,Deserialize)]
pub struct RawSheet{
    #[serde(rename = "Material")]
    material:String,
    #[serde(rename = "Size")]
    size:Vec<u32>,
    #[serde(rename = "Small")]
    small:bool,
    #[serde(rename = "qty")]
    qty:u32
}
