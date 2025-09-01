use std::i32::MAX;

use serde::{Deserialize, Serialize};
use crate::request_data::request_data_v3::RequestDataV3;
use crate::sheet::raw_sheet_v3::RawSheetV3;


#[derive(Serialize, Deserialize)]
pub struct NodeV3{
    #[serde(rename = "Size")]
    size: Vec<i32>,
    #[serde(rename = "Redundancy")]
    redundancy: Vec<i32>,
    #[serde(rename = "NodeType")]
    node_type: String,
    #[serde(rename = "Orient")]
    orient: String,
    #[serde(rename = "CutNum")]
    cut_num: usize,
    #[serde(rename = "Childs")]
    children: Vec<NodeV3>,
}

impl NodeV3 {
    fn from_node(request_data:&RequestDataV3,node:&super::Node)->Self {
        let type_and_id:Vec<&str>=node.node_type.split(":").collect();
        let node_type=type_and_id[0];
        let mut result=NodeV3{size:node.size.clone(),redundancy:vec!(0,0,0),node_type:node_type.to_string(),orient:node.orient.to_string(),cut_num:0,children:vec!()};
        if node.children.len()>0{
            result.cut_num=(node.children.len()-1)/2;
        }
        if node_type=="Part"{
            let id:usize=type_and_id[1].parse().unwrap();
            let raw_part_size:Vec<i32>=request_data.parts.raw_parts[id].size.iter().map(|l|(*l).try_into().unwrap()).collect();
            let mut part_size=Vec::new();
            let mut redundancy=Vec::new();
            let mut max_redundancy=MAX;
            println!("{:?} and {:?}",node.size,raw_part_size);
            for (i0,i1,i2) in vec!((0,1,2),(0,2,1),(1,0,2),(1,2,0),(2,0,1),(2,1,0)){
                let new_size=vec![raw_part_size[i0],raw_part_size[i1],raw_part_size[i2]];
                let new_redundancy: Vec<i32>=vec!(node.size[0]-new_size[0],node.size[1]-new_size[1],node.size[2]-new_size[2]);
                if *new_redundancy.iter().min().unwrap()>=0 && *new_redundancy.iter().max().unwrap()<max_redundancy{
                    part_size=new_size;
                    max_redundancy=*new_redundancy.iter().max().unwrap();
                    redundancy=new_redundancy;
                }
            }
            result.node_type=request_data.sheets.raw_sheets[id].id.clone();
            result.size=part_size;
            result.redundancy=redundancy;
        }
        for child in &node.children{
            result.children.push(Self::from_node(request_data, child));
        }
        result
    }    
}

#[derive(Serialize, Deserialize)]
pub struct BlueprintV3{
    #[serde(rename = "Sheet")]
    sheet: RawSheetV3,
    #[serde(rename = "Root")]
    root: NodeV3,
}

impl BlueprintV3{
    fn from_blueprint(request_data:&RequestDataV3,blueprint:&super::Blueprint)->Self {
        BlueprintV3 { sheet: RawSheetV3 { id: request_data.sheets.raw_sheets[blueprint.sheet.id].id.to_string(), size: blueprint.sheet.size.clone(), small: blueprint.sheet.small, qty: blueprint.sheet.qty }, root: NodeV3::from_node(request_data, &blueprint.root) }
    }
}

#[derive(Serialize,Deserialize)]
pub struct ResponseV3 {
    solution: Vec<BlueprintV3>,
}

impl ResponseV3 {
    pub fn from_response(request_data:&RequestDataV3,response:super::Response)->Self {
        let mut solution=Vec::new();
        for blueprint in response.solution{
            solution.push(BlueprintV3::from_blueprint(request_data, &blueprint));
        }

        ResponseV3 { solution }
    }
}