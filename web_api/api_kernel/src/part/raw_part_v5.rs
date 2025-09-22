use std::collections::HashMap;

use serde::{Deserialize, Serialize};
use crate::part::{AsParts};
use super::raw_part_v4::{RawPartV4,RawPartsV4};

#[derive(Serialize,Deserialize,Clone)]
pub struct RawPartsV5{
    #[serde(rename = "Parts")]
    pub raw_parts:Vec<RawPartV5>,
}

#[derive(Serialize,Deserialize,Clone)]
pub struct RawPartV5{
    #[serde(rename = "ID")]
    pub id:String,
    #[serde(rename = "Size")]
    pub size:Vec<f64>,
    #[serde(rename = "Redundancy")]
    pub redundancy:Vec<f64>,
    #[serde(rename = "Rotatable")]
    pub rotatable:bool,
    #[serde(rename = "MinHardness")]
    pub min_hardness:f64,
    #[serde(rename = "MaxHardness")]
    pub max_hardness:f64,
    #[serde(rename = "qty")]
    pub qty:usize
}

impl RawPartsV5 {
    pub fn to_parts_v4(&self)->RawPartsV4 {
        RawPartsV4{
            raw_parts:self.raw_parts.iter().map(|part|{
                RawPartV4{id:part.id.clone(),size:part.size.clone(),redundancy:part.redundancy.clone(),rotatable:part.rotatable,qty:part.qty}
            }).collect()
        }
        
    }

    pub fn from_parts_v4(part:&RawPartsV4)->Self {
        Self{
            raw_parts:part.raw_parts.iter().map(|part|{
                RawPartV5{id:part.id.clone(),size:part.size.clone(),redundancy:part.redundancy.clone(),rotatable:part.rotatable
                    ,min_hardness:0.,max_hardness:100.,qty:part.qty}
            }).collect()
        }
        
    }

    pub fn regularize(&self)->Self {
        let mut new_parts=vec!();
        let mut id_table=HashMap::<String,usize>::new();
        for next in &self.raw_parts {
            match id_table.entry(next.id.clone()) {
                std::collections::hash_map::Entry::Vacant(e) => {
                    e.insert(new_parts.len());
                    new_parts.push(next.clone());
                }
                std::collections::hash_map::Entry::Occupied(e) => {
                    new_parts[*e.get()].qty += 1;
                }
            }
        }
        Self { raw_parts: new_parts }
    }

    pub fn to_groups(&self)->Vec<(RawPartsV5,f64,f64)> {
        let mut groups=Vec::<(RawPartsV5,f64,f64)>::new();
        for part in &self.raw_parts{
            let mut success=false;
            for (group,min_h,max_h) in &mut groups {
                if *min_h<part.max_hardness && *max_h>part.min_hardness {
                    *min_h=(*min_h).max(part.min_hardness);
                    *max_h=(*max_h).min(part.max_hardness);
                    group.raw_parts.push(part.clone());
                    success=true;
                    break;
                }
            }
            if !success {
                let group=RawPartsV5{raw_parts:vec!(part.clone())};
                groups.push((group,part.min_hardness,part.max_hardness));
            }

        }
        groups
    }

}

impl AsParts for RawPartsV5 {
    fn to_parts(&self)->Vec<super::Part> {
        self.to_parts_v4().to_parts()
    }
}
