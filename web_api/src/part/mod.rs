pub mod raw_part_v3;
pub mod raw_part_v4;

use serde::{Deserialize, Serialize};

#[derive(Serialize,Deserialize)]
pub struct Part{
    #[serde(rename = "ID")]
    pub id:usize,
    #[serde(rename = "Size")]
    pub size:Vec<usize>,
    #[serde(rename = "Rotatable")]
    pub rotatable:bool,
    #[serde(rename = "qty")]
    pub qty:usize
}

pub trait AsParts{
    fn to_parts(self:&Self)->Vec<Part>;
}
