
use serde::{Deserialize, Serialize};
use crate::part::raw_part_v5::{RawPartsV5,RawPartV5};
use crate::request_data::request_data_v5::RequestDataV5;
use crate::request_data::request_data_v4::RequestDataV4;
use super::response_v4::BlueprintV4;
use crate::response::response_v4::{NodeV4, ResponseV4};
use crate::sheet::raw_sheet_v5::RawSheetV5;


#[derive(Serialize, Deserialize)]
pub struct NodeV5{
    #[serde(rename = "Size")]
    pub size: Vec<f64>,
    #[serde(skip_serializing_if = "should_skip_redundancy",rename = "Redundancy")]
    pub redundancy: Vec<f64>,
    #[serde(rename = "NodeType")]
    pub node_type: String,
    #[serde(rename = "Orient")]
    pub orient: String,
    #[serde(rename = "CutNum")]
    pub cut_num: usize,
    #[serde(skip_serializing_if = "should_skip_hardness",rename = "MinHardness")]
    pub min_hardness: f64,
    #[serde(skip_serializing_if = "should_skip_hardness",rename = "MaxHardness")]
    pub max_hardness: f64,
    #[serde(rename = "Childs")]
    pub children: Vec<NodeV5>,
}

fn should_skip_redundancy(node: &Vec<f64>) -> bool {
    let sum:f64=node.iter().sum();
    sum==0.
}

fn should_skip_hardness(h: &f64) -> bool {
    *h==0.
}

impl NodeV5 {
    pub fn from_node_v4(raw_parts_v5:&RawPartsV5,node:&NodeV4)->Self {
        let mut min_h=0.;
        let mut max_h=0.;
        if node.node_type!="Struct" && node.node_type!="Leftover" && node.node_type!="Cutloss" {
            let part=raw_parts_v5.raw_parts.iter().find(|x|(*x).id==node.node_type).unwrap();
            min_h=part.min_hardness;
            max_h=part.max_hardness;
        }
        
        let mut node_v5=NodeV5 { size: node.size.clone(), redundancy: node.redundancy.clone(), 
            node_type: node.node_type.clone(), orient: node.orient.clone(), cut_num: node.cut_num,
            min_hardness:min_h,max_hardness:max_h, children: vec!() };
        for child in &node.children{
            node_v5.children.push(Self::from_node_v4(raw_parts_v5, child));
        }
        node_v5
    }

}

#[derive(Serialize, Deserialize)]
pub struct BlueprintV5{
    #[serde(rename = "Sheet")]
    pub sheet: RawSheetV5,
    #[serde(rename = "Root")]
    pub root: NodeV5,
}

impl BlueprintV5{
    fn from_blueprint_v4(request_data:&RequestDataV5,blueprint_v4:BlueprintV4)->Self {
        let sheet=request_data.sheets.raw_sheets.iter()
        .find(|s|(*s).id==blueprint_v4.sheet.id).unwrap().clone();
        BlueprintV5 { sheet: sheet, 
        root: NodeV5::from_node_v4(&request_data.parts, &blueprint_v4.root) }
    }
}

#[derive(Serialize,Deserialize)]
pub struct ResponseV5 {
    pub solution: Vec<BlueprintV5>,
}

impl ResponseV5 {
    pub fn from_response(request_data:&RequestDataV5,response:super::Response)->Self {
        let request_data_v4=request_data.to_request_data_v4();
        let request_data_v3=request_data_v4.to_request_data_v3();
        let mut solution=Vec::new();
        for blueprint in response.solution{
            let blueprint_v4=BlueprintV4::from_blueprint(&request_data_v4,&request_data_v3, &blueprint);
            solution.push(BlueprintV5::from_blueprint_v4(request_data,blueprint_v4));
        }

        ResponseV5 { solution }
    }
}