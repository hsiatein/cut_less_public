use reqwest::Client;
use serde::Serialize;
use std::env;
use std::fs;

#[derive(Serialize)]
struct RequestData {
    problem: String,
}

#[tokio::main]
async fn main() {
    let client = Client::new();
    let exe_path = env::current_exe().unwrap();
    let root_path = exe_path.parent().unwrap().parent().unwrap().parent().unwrap().parent().unwrap();
    let problem_path = root_path.join("assets").join("test_problem.json");
    let problem  = fs::read_to_string(problem_path).unwrap();
    let request=RequestData{
        problem
    };
    let response = client
        .post("http://localhost:6002/process")
        .json(&request)
        .send()
        .await
        .unwrap();

    println!("{}", response.text().await.unwrap());
}
