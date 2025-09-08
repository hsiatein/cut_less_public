use serde::{Deserialize, Serialize};
use crate::sheet::Sheet;
use crate::sheet::AsSheets;

#[derive(Serialize,Deserialize)]
pub struct RawSheetsV3{
    #[serde(rename = "Sheets")]
    pub raw_sheets:Vec<RawSheetV3>,
}

#[derive(Serialize,Deserialize,Clone)]
pub struct RawSheetV3{
    #[serde(rename = "ID")]
    pub id:String,
    #[serde(rename = "Size")]
    pub size:Vec<usize>,
    #[serde(rename = "Small")]
    pub small:bool,
    #[serde(rename = "qty")]
    pub qty:usize
}

impl AsSheets for RawSheetsV3{
    fn to_sheets(self:&Self)->Vec<super::Sheet> {
        self.raw_sheets.iter().enumerate().map(|pair|{
            let sheet = Sheet{id:pair.0,size:pair.1.size.clone(),small:pair.1.small,qty:pair.1.qty};
            sheet
        }).collect()
    }
}
