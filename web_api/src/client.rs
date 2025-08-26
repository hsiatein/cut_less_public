use reqwest::Client;
use serde::{Serialize,Deserialize};
use std::env;
use std::fs;
use cl_web_api::part::Part;
use cl_web_api::sheet::Sheet;

#[derive(Serialize,Deserialize)]
struct RequestData {
    #[serde(rename = "Parts")]
    parts: Vec<Part>,
    #[serde(rename = "Sheets")]
    sheets: Vec<Sheet>,
}

#[tokio::main]
async fn main() {
    let client = Client::new();
    let exe_path = env::current_exe().unwrap();
    let root_path = exe_path.parent().unwrap().parent().unwrap().parent().unwrap().parent().unwrap();
    let problem_path = root_path.join("assets").join("test_problem.json");
    let problem  = fs::read_to_string(problem_path).unwrap();
    let request:RequestData=serde_json::from_str(&problem).unwrap();
    let response = client
        .post("http://localhost:6002/cut_less")
        .json(&request)
        .send()
        .await
        .unwrap();

    println!("{}", response.text().await.unwrap());
}
