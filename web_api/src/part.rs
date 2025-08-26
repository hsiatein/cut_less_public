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
pub struct RawPart{
    #[serde(rename = "BarCode")]
    bar_code:u32,
    #[serde(rename = "Material")]
    material:String,
    #[serde(rename = "Size")]
    size:Vec<u32>,
    #[serde(rename = "Rotatable")]
    rotatable:bool,
    #[serde(rename = "qty")]
    qty:u32
}