use std::collections::HashMap;

use serde::{Deserialize, Serialize};
use crate::part::{AsParts};
use super::raw_part_v4::{RawPartV4,RawPartsV4};

#[derive(Serialize,Deserialize)]
pub struct RawPartsV5{
    #[serde(rename = "Parts")]
    pub raw_parts:Vec<RawPartV5>,
}

#[derive(Serialize,Deserialize)]
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
                let new_size=part.size.iter().map(|num| ((*num as f64)/crate::FACTOR)).collect();
                RawPartV5{id:part.id.clone(),size:new_size,redundancy:vec!(0.,0.,0.),rotatable:part.rotatable
                    ,min_hardness:0.,max_hardness:100.,qty:part.qty}
            }).collect()
        }
        
    }

    pub fn regularize(&mut self) {
        let mut new_parts=vec!();
        let mut id_table=HashMap::<String,usize>::new();
        while let Some(next) = self.raw_parts.pop() {
            match id_table.entry(next.id.clone()) {
                std::collections::hash_map::Entry::Vacant(e) => {
                    e.insert(new_parts.len());
                    new_parts.push(next);
                }
                std::collections::hash_map::Entry::Occupied(e) => {
                    new_parts[*e.get()].qty += 1;
                }
            }
        }
        new_parts.reverse();
        self.raw_parts=new_parts;
    }
}

impl AsParts for RawPartsV5 {
    fn to_parts(&self)->Vec<super::Part> {
        self.to_parts_v4().to_parts()
    }
}
