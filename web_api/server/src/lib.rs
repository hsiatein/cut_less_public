use api_kernel::{config::AsConfig, request_data::ToProblem};
use api_kernel::response::response_v3::ResponseV3;
use api_kernel::CliError;
use api_kernel::request_data::request_data_v3::RequestDataV3;
use api_kernel::response::response_v4::ResponseV4;
use api_kernel::response::response_v5::ResponseV5;
use api_kernel::request_data::request_data_v4::RequestDataV4;
use api_kernel::request_data::request_data_v5::RequestDataV5;
use api_kernel::response::Response;
use libc::{c_char, c_void};
use std::ffi::{CStr, CString};
use tokio::task;

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
    let problem_cstr = CString::new(problem_string).unwrap();
    let config_cstr = CString::new(config_string).unwrap();
    // println!("{:?}",problem_cstr);
    // println!("{:?}",config_cstr);
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