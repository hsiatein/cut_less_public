use api_kernel::{config::AsConfig, request_data::ToProblem};
use api_kernel::response::response_v3::ResponseV3;
use api_kernel::{response, CliError};
use api_kernel::request_data::request_data_v3::RequestDataV3;
use api_kernel::response::response_v4::ResponseV4;
use api_kernel::response::response_v4_plus::ResponseV4Plus;
use api_kernel::response::response_v5::ResponseV5;
use api_kernel::request_data::request_data_v4::RequestDataV4;
use api_kernel::request_data::request_data_v5::RequestDataV5;
use api_kernel::response::{Blueprint, Metadata, Response};
use libc::{c_char, c_void, rand};
use std::ffi::{CStr, CString};
use tokio::task;
use std::process::{Stdio};
use tokio::process::{Command};
use std::env;
use std::fs;

use futures::future::join_all;
use std::sync::{Arc, Mutex};
use rand::Rng;
use warp::Rejection;

unsafe extern "C" {
    fn solve(problem_string: *const c_char, config_string: *const c_char)->*mut c_void;
    fn get_c_str(result:*mut c_void)->*const c_char;
    fn free_result(result:*mut c_void);
}

pub async fn handle_request(data: RequestDataV3) -> Result<warp::reply::Json, warp::Rejection> {
    println!("start handle");
    let problem_string=serde_json::to_string(&data.to_problem()).unwrap();
    let config_string=serde_json::to_string(&data.config).unwrap();
    let problem_cstr = CString::new(problem_string).unwrap();
    let config_cstr = CString::new(config_string).unwrap();
    // println!("{:?}",problem_cstr);
    // println!("{:?}",config_cstr);
    let solution;
    println!("calculate begin");
    unsafe {
        let result=solve(problem_cstr.as_ptr() as *const c_char, config_cstr.as_ptr() as *const c_char);
        println!("calculate end");
        solution = CStr::from_ptr(get_c_str(result)).to_string_lossy().into_owned();
        println!("get string");
        free_result(result);
        println!("free");
    }
    let solution_json = serde_json::from_str(&solution);
    if let Err(e) = solution_json{
        return Err(warp::reject::custom(CliError(e.to_string())));
    }
    let solution_json=solution_json.unwrap();
    let response = Response {
        solution:solution_json,
    };
    let response_v3=ResponseV3::from_response(&data, response);
    Ok(warp::reply::json(&response_v3))
}

pub async fn handle_request_v4(data: RequestDataV4) -> Result<warp::reply::Json, warp::Rejection> {
    println!("start handle");
    let problem_string=serde_json::to_string(&data.to_problem()).unwrap();
    let config_string=serde_json::to_string(&data.config.to_config()).unwrap();
    println!("{}",problem_string);
    println!("{}",config_string);
    let problem_cstr = CString::new(problem_string).unwrap();
    let config_cstr = CString::new(config_string).unwrap();

    let solution;
    unsafe {
        println!("calculate begin");
        let result=solve(problem_cstr.as_ptr() as *const c_char, config_cstr.as_ptr() as *const c_char);
        println!("calculate end");
        solution = CStr::from_ptr(get_c_str(result)).to_string_lossy().into_owned();
        println!("get string");
        free_result(result);
        println!("free");
    }
    let solution_json = serde_json::from_str(&solution);
    if let Err(e) = solution_json{
        return Err(warp::reject::custom(CliError(e.to_string())));
    }
    let solution_json=solution_json.unwrap();
    let response = Response {
        solution:solution_json,
    };
    let response_v4=ResponseV4::from_response(&data, response);
    Ok(warp::reply::json(&response_v4))
}


pub async fn handle_request_v4_cli(data: RequestDataV4) -> Result<warp::reply::Json, warp::Rejection> {
    let exe_path = env::current_exe().unwrap();
    let root_path=exe_path.parent().unwrap();
    let cl_path = root_path.join("main_json");
    let output_path = root_path.join("output/");
    // let config = SolverConfig::default();
    let name="solution";
    // 调用cut_less
    let output = Command::new(cl_path)
        .arg("-c").arg(serde_json::to_string(&data.config.to_config()).unwrap())
        .arg("-o").arg(output_path.to_str().unwrap())
        .arg("-p").arg(serde_json::to_string(&data.to_problem()).unwrap())
        .arg("-n").arg(name)
        .stdout(Stdio::piped())
        .output().await;

    match output {
        Ok(output) if output.status.success() => {
            // 获得response
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
            let response = Response {
                solution:solution_json,
            };
            // 处理response
            let response_v4=ResponseV4::from_response(&data, response);
            Ok(warp::reply::json(&response_v4))
        }
        Ok(output) => {
            // println!("{}",serde_json::to_string_pretty(&data.config.to_config()).unwrap());
            // println!("{}",serde_json::to_string_pretty(&data.to_problem()).unwrap());
            let error_msg = String::from_utf8_lossy(&output.stderr).to_string();
            println!("{}",str::from_utf8(&output.stdout).unwrap());
            println!("stderr");
            println!("{}",error_msg);
            Err(warp::reject::reject())
        }
        Err(e) => {
            println!("执行失败");
            println!("{}",e);
            Err(warp::reject::reject())
        }
    }
}

pub async fn handle_request_v4_cli_mt(data: RequestDataV4) -> Result<warp::reply::Json, Rejection> {
    let exe_path = env::current_exe().unwrap();
    let root_path = exe_path.parent().unwrap();
    let cl_path = root_path.join("main_json");
    let output_path = root_path.join("output/");

    // 准备所有任务
    let params:Vec<(usize,usize)>=match data.config.mode {
        0 => {
            [1,2,3,4,6,8].iter().flat_map(|&i|{
                [(i,0)]
            }).collect()
        },
        1 => {
            [1,2,3,4,6,8].iter().flat_map(|&i|{
                [(i,0),(i,1)]
            }).collect()
        },
        _ => {
            vec![(data.config.max_stage,0)]
        }
    };
    let name_prefix:usize=rand::thread_rng().r#gen();
    
    let tasks = params.iter().enumerate().map(|(i,&(stage,mode))| {
        let cl_path = cl_path.clone();
        let output_path = output_path.clone();
        let data = data.clone();
        let seed = rand::thread_rng().r#gen();
        tokio::spawn(async move {
            let name = format!("{}_{}",name_prefix,i);
            let mut config = data.config.to_config();
            config.mode=mode;
            config.max_stage = stage;
            config.lns_random_seed = seed;

            let output = Command::new(&cl_path)
                .arg("-c").arg(serde_json::to_string(&config).unwrap())
                .arg("-o").arg(output_path.to_str().unwrap())
                .arg("-p").arg(serde_json::to_string(&data.to_problem()).unwrap())
                .arg("-n").arg(&name)
                .stdout(Stdio::piped())
                .stderr(Stdio::piped())
                .output()
                .await;

            (i, output)
        })
    });

    // 并发执行所有任务
    let results = join_all(tasks).await;

    let mut best_response: Option<Response> = None;
    let mut best_metadata = Metadata::new();

    // 遍历任务结果
    for result in results {
        if let Ok((i, Ok(output))) = result {
            if output.status.success() {
                println!("Task {i} OK: {}", String::from_utf8_lossy(&output.stdout));
                let solution_path = output_path.join(format!("main@{}_{}.json", name_prefix,i));
                let metadata_path = output_path.join(format!("main@{}_{}_metadata.json", name_prefix,i));
                
                if let (Ok(solution), Ok(metadata)) = (
                    fs::read_to_string(&solution_path),
                    fs::read_to_string(&metadata_path),
                ) {
                    if let (Ok(sol), Ok(meta)) =
                        (serde_json::from_str::<Vec<Blueprint>>(&solution),
                         serde_json::from_str::<Metadata>(&metadata))
                    {
                        if best_response.is_none() || meta.greater(&best_metadata) {
                            best_metadata = meta;
                            best_response = Some(Response { solution: sol });
                        }
                    }
                    fs::remove_file(&solution_path).unwrap();
                    fs::remove_file(&metadata_path).unwrap();
                }
            } else {
                println!("Task {i} failed: {}", String::from_utf8_lossy(&output.stderr));
            }
        }
    }

    if let Some(resp) = best_response {
        let mut response_v4 = ResponseV4::from_response(&data, resp);
        best_metadata.regularize();
        response_v4.metadata = best_metadata;
        Ok(warp::reply::json(&response_v4))
    } else {
        Err(warp::reject::reject())
    }
}

pub async fn handle_request_v4_cli_mt_multi(data: RequestDataV4) -> Result<warp::reply::Json, Rejection> {
    let exe_path = env::current_exe().unwrap();
    let root_path = exe_path.parent().unwrap();
    let cl_path = root_path.join("main_json");
    let output_path = root_path.join("output/");

    let params: Vec<(usize, usize)> = match data.config.mode {
        0 => [1, 2, 3, 4, 6, 8].iter().map(|&i| (i, 0)).collect(),
        1 => [1, 2, 3, 4, 6, 8]
            .iter()
            .flat_map(|&i| [(i, 0), (i, 1)])
            .collect(),
        _ => vec![(data.config.max_stage, 0)],
    };

    let name_prefix: usize = rand::thread_rng().r#gen();

    let tasks = params.iter().enumerate().map(|(i, &(stage, mode))| {
        let cl_path = cl_path.clone();
        let output_path = output_path.clone();
        let data = data.clone();
        let seed = rand::thread_rng().r#gen();
        tokio::spawn(async move {
            let name = format!("{}_{}", name_prefix, i);
            let mut config = data.config.to_config();
            config.mode = mode;
            config.max_stage = stage;
            config.lns_random_seed = seed;

            let output = Command::new(&cl_path)
                .arg("-c")
                .arg(serde_json::to_string(&config).unwrap())
                .arg("-o")
                .arg(output_path.to_str().unwrap())
                .arg("-p")
                .arg(serde_json::to_string(&data.to_problem()).unwrap())
                .arg("-n")
                .arg(&name)
                .stdout(Stdio::piped())
                .stderr(Stdio::piped())
                .output()
                .await;

            (i, output)
        })
    });

    let results = join_all(tasks).await;
    let mut collected = Vec::new();

    for result in results {
        if let Ok((i, Ok(output))) = result {
            if output.status.success() {
                println!(
                    "Task {i} OK: {}",
                    String::from_utf8_lossy(&output.stdout)
                );
                let solution_path =
                    output_path.join(format!("main@{}_{}.json", name_prefix, i));
                let metadata_path =
                    output_path.join(format!("main@{}_{}_metadata.json", name_prefix, i));

                match (
                    fs::read_to_string(&solution_path),
                    fs::read_to_string(&metadata_path),
                ) {
                    (Ok(solution), Ok(metadata)) => {
                        let parsed_solution =
                            serde_json::from_str::<Vec<Blueprint>>(&solution);
                        let parsed_metadata =
                            serde_json::from_str::<Metadata>(&metadata);

                        if let (Ok(sol), Ok(mut meta)) =
                            (parsed_solution, parsed_metadata)
                        {
                            meta.regularize();
                            let mut response_v4 =
                                ResponseV4::from_response(&data, Response { solution: sol });
                            response_v4.metadata = meta;
                            collected.push(response_v4);
                        }
                    }
                    _ => {}
                }

                let _ = fs::remove_file(&solution_path);
                let _ = fs::remove_file(&metadata_path);
            } else {
                println!(
                    "Task {i} failed: {}",
                    String::from_utf8_lossy(&output.stderr)
                );
            }
        }
    }

    if collected.is_empty() {
        Err(warp::reject::reject())
    } else {
        Ok(warp::reply::json(&ResponseV4Plus::from_responses(collected)))
    }
}


// pub async fn handle_request_v4_cli_mt(data: RequestDataV4) -> Result<warp::reply::Json, warp::Rejection> {
//     let exe_path = env::current_exe().unwrap();
//     let root_path=exe_path.parent().unwrap();
//     let cl_path = root_path.join("main_json");
//     let output_path = root_path.join("output/");
//     // let config = SolverConfig::default();
//     // 调用cut_less
//     let mut best_response:Option<Response>=None;
//     let mut best_metadata=Metadata::new();
//     for i in [1,6]{
//         let name=i.to_string();
//         let mut config=data.config.to_config();
//         config.max_stage=i;
//         let output = Command::new(&cl_path)
//             .arg("-c").arg(serde_json::to_string(&config).unwrap())
//             .arg("-o").arg(output_path.to_str().unwrap())
//             .arg("-p").arg(serde_json::to_string(&data.to_problem()).unwrap())
//             .arg("-n").arg(&name)
//             .stdout(Stdio::piped())
//             .output().await;

//         match output {
//             Ok(output) if output.status.success() => {
//                 // 获得response
//                 println!("{}",str::from_utf8(&output.stdout).unwrap());
//                 let solution_path = output_path.join(format!("main@{}.json",name));
//                 let metadata_path = output_path.join(format!("main@{}_metadata.json",name));
//                 let solution  = fs::read_to_string(solution_path);
//                 let metadata  = fs::read_to_string(metadata_path);
//                 if let Err(e) = solution{
//                     println!("{}",e);
//                     continue;
//                 }
//                 if let Err(e) = metadata{
//                     println!("{}",e);
//                     continue;
//                 } 
//                 let solution=solution.unwrap();
//                 let metadata = metadata.unwrap();
//                 let solution_json = serde_json::from_str(&solution);
//                 let metadata_json = serde_json::from_str::<Metadata>(&metadata);
//                 if let Err(e) = solution_json{
//                     println!("{}",e);
//                     continue;
//                 }
//                 if let Err(e) = metadata_json{
//                     println!("{}",e);
//                     continue;
//                 }
//                 let metadata_json=metadata_json.unwrap();
//                 if best_response.is_none() || metadata_json.greater(&best_metadata){
//                     best_metadata=metadata_json;
//                     let solution_json=solution_json.unwrap();
//                     best_response=Some(Response {
//                         solution:solution_json,
//                     });
//                 }
                
//             }
//             Ok(output) => {
//                 // println!("{}",serde_json::to_string_pretty(&data.config.to_config()).unwrap());
//                 // println!("{}",serde_json::to_string_pretty(&data.to_problem()).unwrap());
//                 let error_msg = String::from_utf8_lossy(&output.stderr).to_string();
//                 println!("{}",str::from_utf8(&output.stdout).unwrap());
//                 println!("stderr");
//                 println!("{}",error_msg);
//             }
//             Err(e) => {
//                 println!("执行失败");
//                 println!("{}",e);
//             }
//         }
        
//     }
//     // 处理response
//     if best_response.is_none(){
//         return Err(warp::reject::reject());
//     }
//     let response_v4=ResponseV4::from_response(&data, best_response.unwrap());
//     Ok(warp::reply::json(&response_v4))

// }

pub async fn handle_request_v5_single(data: RequestDataV5) -> Result<warp::reply::Json, warp::Rejection> {
    println!("start handle");
    let groups = data.split();
    let mut response_result = ResponseV5 { solution: vec![] };

    for group in groups {
        let handle = task::spawn_blocking(move || {
            let problem_string = serde_json::to_string(&group.to_problem()).unwrap();
            let config_string = serde_json::to_string(&group.config.to_config()).unwrap();
            println!("{}",problem_string);
            println!("{}",config_string);
            let problem_cstr = CString::new(problem_string).unwrap();
            let config_cstr = CString::new(config_string).unwrap();
            let solution;
            unsafe {
                println!("calculate begin");
                let result = solve(problem_cstr.as_ptr() as *const c_char, config_cstr.as_ptr() as *const c_char);
                println!("calculate end");
                solution = CStr::from_ptr(get_c_str(result)).to_string_lossy().into_owned();
                println!("free");
                free_result(result);
            }
            let solution_json = serde_json::from_str(&solution).unwrap();
            let response = Response { solution: solution_json };
            ResponseV5::from_response(&group, response)
        });
        let response = handle.await.unwrap(); // join handle
        response_result.solution.extend(response.solution);
    }

    Ok(warp::reply::json(&response_result))
}

pub async fn handle_request_v5(data: RequestDataV5) -> Result<warp::reply::Json, warp::Rejection> {
    println!("start handle");
    let groups = data.split();

    let mut handles = Vec::new();

    for group in groups {
        let handle = task::spawn_blocking(move || {
            let problem_string = serde_json::to_string(&group.to_problem()).unwrap();
            println!("{}",problem_string);
            let config_string = serde_json::to_string(&group.config.to_config()).unwrap();
            let problem_cstr = CString::new(problem_string).unwrap();
            let config_cstr = CString::new(config_string).unwrap();
            let solution;
            unsafe {
                println!("calculate begin");
                let result = solve(problem_cstr.as_ptr() as *const c_char, config_cstr.as_ptr() as *const c_char);
                println!("calculate end");
                solution = CStr::from_ptr(get_c_str(result)).to_string_lossy().into_owned();
                println!("free");
                free_result(result);
            }
            let solution_json = serde_json::from_str(&solution).unwrap();
            let response = Response { solution: solution_json };
            ResponseV5::from_response(&group, response)
        });
        handles.push(handle);

    }

    let mut response_result = ResponseV5 { solution: vec![] };

    for h in handles {
        let response = h.await.unwrap(); // join handle
        response_result.solution.extend(response.solution);
    }

    Ok(warp::reply::json(&response_result))
}
