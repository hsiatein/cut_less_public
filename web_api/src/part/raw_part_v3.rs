use serde::{Deserialize, Serialize};
use crate::part::{AsParts, Part};

#[derive(Serialize,Deserialize)]
pub struct RawPartsV3{
    #[serde(rename = "Parts")]
    pub raw_parts:Vec<RawPartV3>,
}

#[derive(Serialize,Deserialize)]
pub struct RawPartV3{
    #[serde(rename = "ID")]
    pub id:String,
    #[serde(rename = "Size")]
    pub size:Vec<usize>,
    #[serde(rename = "Rotatable")]
    pub rotatable:bool,
    #[serde(rename = "qty")]
    pub qty:usize
}

impl AsParts for RawPartsV3 {
    fn to_parts(&self)->Vec<super::Part> {
        self.raw_parts.iter().enumerate().map(|pair|{
            let part = Part{id:pair.0,size:pair.1.size.clone(),rotatable:pair.1.rotatable,qty:pair.1.qty};
            part
        }).collect()
    }
}
