pub mod raw_sheet_v3;

use serde::{Deserialize, Serialize};

#[derive(Serialize,Deserialize)]
pub struct Sheet{
    #[serde(rename = "ID")]
    pub id:usize,
    #[serde(rename = "Size")]
    pub size:Vec<usize>,
    #[serde(rename = "Small")]
    pub small:bool,
    #[serde(default="default_qty",rename = "qty")]
    pub qty:usize
}

pub trait AsSheets{
    fn to_sheets(self:&Self)->Vec<Sheet>;
}

pub fn default_qty()->usize{
    1
}
