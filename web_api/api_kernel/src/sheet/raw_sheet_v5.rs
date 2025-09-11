use serde::{Deserialize, Serialize};

use crate::part::raw_part_v5::RawPartsV5;
use crate::sheet::AsSheets;
use crate::sheet::raw_sheet_v4::{RawSheetV4,RawSheetsV4};

#[derive(Serialize,Deserialize)]
pub struct RawSheetsV5{
    #[serde(rename = "Sheets")]
    pub raw_sheets:Vec<RawSheetV5>,
}

#[derive(Serialize,Deserialize,Clone)]
pub struct RawSheetV5{
    #[serde(rename = "ID")]
    pub id:String,
    #[serde(rename = "Size")]
    pub size:Vec<f64>,
    #[serde(rename = "Small")]
    pub small:bool,
    #[serde(rename = "MinHardness")]
    pub min_hardness:f64,
    #[serde(rename = "MaxHardness")]
    pub max_hardness:f64,
    #[serde(rename = "qty")]
    pub qty:usize
}

impl RawSheetsV5{
    pub fn to_sheets_v4(&self)->RawSheetsV4 {
        RawSheetsV4{
            raw_sheets:self.raw_sheets.iter().map(|sheet|{
                RawSheetV4{id:sheet.id.clone(),size:sheet.size.clone(),small:sheet.small,qty:sheet.qty}
            }).collect()
        }
        
    }

    pub fn from_sheets_v4(part:&RawSheetsV4)->Self {
        Self{
            raw_sheets:part.raw_sheets.iter().map(|sheet|{
                RawSheetV5{id:sheet.id.clone(),size:sheet.size.clone(),small:sheet.small,
                    min_hardness:0.,max_hardness:100.,qty:sheet.qty}
            }).collect()
        }
        
    }

    pub fn to_groups(&self,part_groups:Vec<(RawPartsV5,f64,f64)>)->Vec<(RawPartsV5,RawSheetsV5)> {
        let mut groups=vec!();
        for (part_group,min_h,max_h) in part_groups {
            let mut group=(part_group,RawSheetsV5{raw_sheets:vec!()});
            for sheet in &self.raw_sheets {
                if sheet.min_hardness>=min_h && sheet.max_hardness<=max_h {
                    group.1.raw_sheets.push(sheet.clone());
                }
            }
            groups.push(group);
        }
        groups
    }
}


impl AsSheets for RawSheetsV5{
    fn to_sheets(self:&Self)->Vec<super::Sheet> {
        self.to_sheets_v4().to_sheets()
    }
}
