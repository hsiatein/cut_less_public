use warp::Filter;
use serde::{Deserialize, Serialize};
use std::process::{Command, Stdio};
use std::env;
use std::fs;

#[derive(Debug)]
struct CliError(String);
impl warp::reject::Reject for CliError {
    
}

#[derive(Deserialize)]
struct RequestData {
    problem: String,
}

#[derive(Serialize,Deserialize)]
struct ApiResponse {
    solution: Vec<Node>,
}

#[derive(Serialize, Deserialize, Debug)]
struct Node {
    #[serde(rename = "Size")]
    size: Vec<i32>,
    #[serde(rename = "NodeType")]
    node_type: String,
    #[serde(rename = "Orient")]
    orient: String,
    #[serde(rename = "Childs")]
    children: Vec<Node>,
}

#[tokio::main]
async fn main() {
    let process_route = warp::post()
        .and(warp::path("process"))
        .and(warp::body::json())
        .and_then(handle_request);

    warp::serve(process_route)
        .run(([0, 0, 0, 0], 6002))
        .await;
}

async fn handle_request(data: RequestData) -> Result<warp::reply::Json, warp::Rejection> {
    let exe_path = env::current_exe().unwrap();
    let root_path = exe_path.parent().unwrap().parent().unwrap().parent().unwrap().parent().unwrap();
    let cl_path = root_path.join("build").join("main_json");
    let output_path = root_path.join("web_api").join("output/");
    let solution_path = output_path.join("main@solution.json");
    let config_path = root_path.join("config.json");
    
    // 调用cut_less
    let output = Command::new(cl_path)
        .arg("-c").arg(config_path.to_str().unwrap())
        .arg("-o").arg(output_path.to_str().unwrap())
        .arg("-p").arg(data.problem)
        .stdout(Stdio::piped())
        .output();

    match output {
        Ok(output) if output.status.success() => {
            let solution  = fs::read_to_string(solution_path);
            if let Err(e) = solution{
                return Err(warp::reject::custom(CliError(e.to_string())));
            }
            let solution=solution.unwrap();
            let solution_json = serde_json::from_str(&solution);
            if let Err(e) = solution_json{
                return Err(warp::reject::custom(CliError(e.to_string())));
            }
            let solution_json=solution_json.unwrap();
            let response = ApiResponse {
                solution:solution_json,
            };
            Ok(warp::reply::json(&response))
        }
        Ok(output) => {
            let error_msg = String::from_utf8_lossy(&output.stderr).to_string();
            Err(warp::reject::custom(CliError(error_msg)))
        }
        Err(e) => {
            Err(warp::reject::custom(CliError(e.to_string())))
        }
    }
}