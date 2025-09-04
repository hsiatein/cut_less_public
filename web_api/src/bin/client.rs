use cl_web_api::request_data::request_data_v3::RequestDataV3;
use reqwest::Client;
use std::env;
use std::fs;
use cl_web_api::request_data::RequestData;

#[tokio::main]
async fn main() {
    let client = Client::new();
    let exe_path = env::current_exe().unwrap();
    let root_path = exe_path.parent().unwrap().parent().unwrap().parent().unwrap().parent().unwrap();
    let problem_path = root_path.join("assets").join("v3").join("test_problem.json");
    let problem  = fs::read_to_string(problem_path).unwrap();
    let request:RequestDataV3=serde_json::from_str(&problem).unwrap();
    let response = client
        .post("http://localhost:6002/cut_less")
        .json(&request)
        .send()
        .await
        .unwrap();

    println!("{}", response.text().await.unwrap());
}
