
use serde::{Deserialize, Serialize};
use crate::part::raw_part_v4::{RawPartsV4,RawPartV4};
use crate::request_data::request_data_v4::RequestDataV4;
use crate::request_data::request_data_v3::RequestDataV3;
use crate::response::response_v3::NodeV3;
use crate::sheet::raw_sheet_v4::RawSheetV4;


#[derive(Serialize, Deserialize)]
pub struct NodeV4{
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
    #[serde(rename = "Childs")]
    pub children: Vec<NodeV4>,
}

fn should_skip_redundancy(node: &Vec<f64>) -> bool {
    let sum:f64=node.iter().sum();
    sum==0.
}

impl NodeV4 {
    pub fn from_node_v3(raw_parts_v4:&RawPartsV4,node:&NodeV3)->Self {
        let new_size;
        let redundancy;
        if node.node_type!="Struct" && node.node_type!="Leftover" && node.node_type!="Cutloss" {
            let new_red;
            let raw_part_v4=raw_parts_v4.raw_parts.iter()
            .find(|part| {
                // println!("{},{}",part.id,node.node_type);
                part.id==node.node_type
            }).unwrap();
            let base_redundancy:Vec<f64>=node.redundancy.iter().map(|x|(*x as f64)/crate::FACTOR).collect();
            (new_size,new_red)=Self::modify_order(raw_part_v4, &node.size);
            redundancy=base_redundancy.iter().zip(new_red).map(|pair|*pair.0+pair.1).collect();
        }
        else {
            new_size=node.size.iter().map(|x| (*x as f64)/crate::FACTOR).collect();
            redundancy=node.redundancy.iter().map(|x| (*x as f64)/crate::FACTOR).collect();
        }
        
        let mut node_v4=NodeV4 { size: new_size, redundancy: redundancy, node_type: node.node_type.clone(), orient: node.orient.clone(), cut_num: node.cut_num, children: vec!() };
        for child in &node.children{
            node_v4.children.push(Self::from_node_v3(raw_parts_v4, child));
        }
        node_v4
    }
    pub fn modify_order(raw_part:&RawPartV4,size:&Vec<i32>)->(Vec<f64>,Vec<f64>){
        let mut best_score=1e100;
        let mut best_size=vec!();
        let mut best_red=vec!();
        for (i0,i1,i2) in vec!((0,1,2),(0,2,1),(1,0,2),(1,2,0),(2,0,1),(2,1,0)){
            let new_size=vec!(raw_part.size[i0],raw_part.size[i1],raw_part.size[i2]);
            let new_red=vec!(raw_part.redundancy[i0],raw_part.redundancy[i1],raw_part.redundancy[i2]);
            let score=new_size.iter().zip(new_red.iter()).zip(size.iter()).map(|pair| ((*pair.0.0+*pair.0.1)*crate::FACTOR-(*pair.1 as f64)).abs()).sum::<f64>();
            if score<best_score {
                best_score=score;
                best_size=new_size;
                best_red=new_red;
            }
        }
        (best_size,best_red)
    }
}

#[derive(Serialize, Deserialize)]
pub struct BlueprintV4{
    #[serde(rename = "Sheet")]
    sheet: RawSheetV4,
    #[serde(rename = "Root")]
    root: NodeV4,
}

impl BlueprintV4{
    fn from_blueprint(request_data:&RequestDataV4,request_data_v3:&RequestDataV3,blueprint:&super::Blueprint)->Self {
        let node_v3=NodeV3::from_node(&request_data_v3.parts, &blueprint.root);
        BlueprintV4 { sheet: request_data.sheets.raw_sheets[blueprint.sheet.id].clone(), 
        root: NodeV4::from_node_v3(&request_data.parts, &node_v3) }
    }
}

#[derive(Serialize,Deserialize)]
pub struct ResponseV4 {
    solution: Vec<BlueprintV4>,
}

impl ResponseV4 {
    pub fn from_response(request_data:&RequestDataV4,response:super::Response)->Self {
        let mut solution=Vec::new();
        let request_data_v3=request_data.to_request_data_v3();
        for blueprint in response.solution{
            solution.push(BlueprintV4::from_blueprint(request_data,&request_data_v3, &blueprint));
        }

        ResponseV4 { solution }
    }
}