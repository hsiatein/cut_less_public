pub mod response_v3;
pub mod response_v4;
pub mod response_v5;

use serde::{Deserialize, Serialize};
use crate::sheet::Sheet;

#[derive(Serialize, Deserialize, Debug)]
pub struct Node {
    #[serde(rename = "Size")]
    size: Vec<i32>,
    #[serde(rename = "NodeType")]
    node_type: String,
    #[serde(rename = "Orient")]
    orient: String,
    #[serde(rename = "Childs")]
    children: Vec<Node>,
}


#[derive(Serialize, Deserialize)]
pub struct Blueprint{
    #[serde(rename = "Sheet")]
    pub sheet: Sheet,
    #[serde(rename = "Root")]
    pub root: Node,
}

#[derive(Serialize,Deserialize)]
pub struct Metadata{
    #[serde(rename = "TotalVolume")]
    pub total_volume: f64,
    #[serde(rename = "UtilVolume")]
    pub util_volume: f64,
    #[serde(rename = "UtilRate")]
    pub util_rate: f64,
    #[serde(rename = "PartsNum")]
    pub parts_num: usize,
    #[serde(rename = "SheetsNum")]
    pub sheets_num: usize,
    #[serde(rename = "CutsNum")]
    pub cuts_num: usize,
}

impl Metadata {
    pub fn new()->Self {
        Metadata { total_volume: 0., util_volume: 0., util_rate: 0., parts_num: 0, sheets_num: 0, cuts_num: 0 }
    }

    pub fn regularize(&mut self) {
        self.total_volume=(self.total_volume/crate::FACTOR.powi(3)).round();
        self.util_volume=(self.util_volume/crate::FACTOR.powi(3)).round();
        self.util_rate=(self.util_rate*1e3).round()/1e3;
    }

    pub fn greater(&self,other:&Self)->bool {
        if self.parts_num!=other.parts_num {
            self.parts_num>other.parts_num
        }
        else if self.total_volume!=other.total_volume {
            self.total_volume<other.total_volume
        }
        else if self.cuts_num!=other.cuts_num {
            self.cuts_num<other.cuts_num
        }
        else {
            false
        }
    }
}

#[derive(Serialize,Deserialize)]
pub struct Response{
    pub solution: Vec<Blueprint>,
}
