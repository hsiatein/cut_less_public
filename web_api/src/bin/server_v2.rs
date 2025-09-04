use warp::Filter;
use serde::{Deserialize, Serialize};
use std::process::{Stdio};
use tokio::process::{Command};
use std::env;
use std::fs;
use cl_web_api::CliError;
use cl_web_api::request_data::RequestData;
use cl_web_api::response::Node;

#[derive(Serialize,Deserialize)]
struct ApiResponse {
    solution: Vec<Node>,
}

#[tokio::main]
async fn main() {
    let process_route = warp::post()
        .and(warp::path("cut_less"))
        .and(warp::body::json())
        .and_then(handle_request);

    warp::serve(process_route)
        .run(([0, 0, 0, 0], 6002))
        .await;
}

async fn handle_request(data: RequestData) -> Result<warp::reply::Json, warp::Rejection> {
    let exe_path = env::current_exe().unwrap();
    let root_path=exe_path.parent().unwrap();
    let cl_path = root_path.join("main_json");
    let output_path = root_path.join("output/");
    // let config = SolverConfig::default();
    let name="solution";
    
    // 调用cut_less
    let output = Command::new(cl_path)
        .arg("-c").arg(serde_json::to_string(&data.config).unwrap())
        .arg("-o").arg(output_path.to_str().unwrap())
        .arg("-p").arg(serde_json::to_string(&data).unwrap())
        .arg("-n").arg(name)
        .stdout(Stdio::piped())
        .output().await;

    match output {
        Ok(output) if output.status.success() => {
            println!("{}",str::from_utf8(&output.stdout).unwrap());
            let solution_path = output_path.join(format!("main@{}.json",name));
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
            println!("stderr");
            println!("{}",error_msg);
            Err(warp::reject::custom(CliError(error_msg)))
        }
        Err(e) => {
            println!("执行失败");
            Err(warp::reject::custom(CliError(e.to_string())))
        }
    }
}