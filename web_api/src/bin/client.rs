use api_kernel::request_data::request_data_v4::RequestDataV4;
use reqwest::Client;
use std::env;
use std::fs;

#[tokio::main]
async fn main() {
    let client = Client::new();
    let exe_path = env::current_exe().unwrap();
    let root_path = exe_path.parent().unwrap().parent().unwrap().parent().unwrap().parent().unwrap();
    let problem_path = root_path.join("assets").join("v4").join("input4.json");
    let problem  = fs::read_to_string(problem_path).unwrap();
    let request:RequestDataV4=serde_json::from_str(&problem).unwrap();
    let response = client
        .post("http://localhost:6002/cut_less")
        .json(&request)
        .send()
        .await
        .unwrap();

    println!("{}", response.text().await.unwrap());
}
