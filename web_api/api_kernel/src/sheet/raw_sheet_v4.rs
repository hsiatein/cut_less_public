use serde::{Deserialize, Serialize};

use crate::sheet::AsSheets;
use crate::sheet::raw_sheet_v3::{RawSheetV3,RawSheetsV3};

#[derive(Serialize,Deserialize)]
pub struct RawSheetsV4{
    #[serde(rename = "Sheets")]
    pub raw_sheets:Vec<RawSheetV4>,
}

#[derive(Serialize,Deserialize,Clone)]
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

impl RawSheetsV4{
    pub fn to_sheets_v3(&self)->RawSheetsV3 {
        RawSheetsV3{
            raw_sheets:self.raw_sheets.iter().map(|sheet|{
                let new_size=sheet.size.iter().map(|num| (*num*crate::FACTOR).round() as usize).collect();
                RawSheetV3{id:sheet.id.clone(),size:new_size,small:sheet.small,qty:sheet.qty}
            }).collect()
        }
        
    }

    pub fn from_sheets_v3(part:&RawSheetsV3)->Self {
        Self{
            raw_sheets:part.raw_sheets.iter().map(|sheet|{
                let new_size=sheet.size.iter().map(|num| (*num as f64)/crate::FACTOR).collect();
                RawSheetV4{id:sheet.id.clone(),size:new_size,small:sheet.small,qty:sheet.qty}
            }).collect()
        }
        
    }
}


impl AsSheets for RawSheetsV4{
    fn to_sheets(self:&Self)->Vec<super::Sheet> {
        self.to_sheets_v3().to_sheets()
    }
}
