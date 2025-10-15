use serde::{Deserialize, Serialize};
use crate::part::{AsParts};
use super::raw_part_v3::{RawPartV3,RawPartsV3};
use std::collections::HashMap;

#[derive(Serialize,Deserialize,Clone)]
pub struct RawPartsV4{
    #[serde(rename = "Parts")]
    pub raw_parts:Vec<RawPartV4>,
}

#[derive(Serialize,Deserialize,Clone)]
pub struct RawPartV4{
    #[serde(rename = "ID")]
    pub id:String,
    #[serde(rename = "Size")]
    pub size:Vec<f64>,
    #[serde(rename = "Redundancy")]
    pub redundancy:Vec<f64>,
    #[serde(rename = "Rotatable")]
    pub rotatable:bool,
    #[serde(rename = "qty")]
    pub qty:usize
}

impl RawPartsV4 {
    pub fn to_parts_v3(&self)->RawPartsV3 {
        RawPartsV3{
            raw_parts:self.raw_parts.iter().map(|part|{
                let new_size=part.size.iter().zip(part.redundancy.clone()).map(|num| ((*num.0+num.1)*crate::FACTOR).round() as usize).collect();
                RawPartV3{id:part.id.clone(),size:new_size,rotatable:part.rotatable,qty:part.qty}
            }).collect()
        }
        
    }

    pub fn from_parts_v3(part:&RawPartsV3)->Self {
        Self{
            raw_parts:part.raw_parts.iter().map(|part|{
                let new_size=part.size.iter().map(|num| ((*num as f64)/crate::FACTOR)).collect();
                RawPartV4{id:part.id.clone(),size:new_size,redundancy:vec!(0.,0.,0.),rotatable:part.rotatable,qty:part.qty}
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

}

impl AsParts for RawPartsV4 {
    fn to_parts(&self)->Vec<super::Part> {
        self.to_parts_v3().to_parts()
    }
}
