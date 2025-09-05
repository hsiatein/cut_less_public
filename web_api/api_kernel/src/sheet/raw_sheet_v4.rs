use serde::{Deserialize, Serialize};

use crate::sheet::Sheet;
use crate::sheet::AsSheets;

#[derive(Serialize,Deserialize)]
pub struct RawSheetsV4{
    #[serde(rename = "Sheets")]
    pub raw_sheets:Vec<RawSheetV4>,
}

#[derive(Serialize,Deserialize)]
pub struct RawSheetV4{
    #[serde(rename = "ID")]
    pub id:String,
    #[serde(rename = "Size")]
    pub size:Vec<f64>,
    #[serde(rename = "Small")]
    pub small:bool,
    #[serde(rename = "qty")]
    pub qty:usize
}



impl AsSheets for RawSheetsV4{
    fn to_sheets(self:&Self)->Vec<super::Sheet> {
        self.raw_sheets.iter().enumerate().map(|pair|{
            let new_size=pair.1.size.iter().map(|num| (*num*crate::FACTOR).round() as usize).collect();
            Sheet{id:pair.0,size:new_size,small:pair.1.small,qty:pair.1.qty}
        }).collect()
    }
}
