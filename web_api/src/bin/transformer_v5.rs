use api_kernel::config::config_v4::SolverConfigV4;
use api_kernel::part::raw_part_v5::RawPartsV5;
use api_kernel::sheet::raw_sheet_v5::RawSheetsV5;
use std::env;
use std::fs;
use api_kernel::request_data::request_data_v5::RequestDataV5;
use api_kernel::request_data::request_data_v4::RequestDataV4;

#[tokio::main]
async fn main() {
    let problems=vec!("test_problem.json","test_problem2.json","test_problem3.json","test_problem4.json","input.json");
    let exe_path = env::current_exe().unwrap();
    let root_path = exe_path.parent().unwrap().parent().unwrap().parent().unwrap().parent().unwrap();
    for problem_name in &problems{
        let problem_path = root_path.join("assets/v4").join(problem_name);
        let problem  = fs::read_to_string(problem_path).unwrap();
        let request:RequestDataV4=serde_json::from_str(&problem).unwrap();
        let new_request=RequestDataV5{config:SolverConfigV4::default()
            ,parts:RawPartsV5::from_parts_v4(&request.parts)
            ,sheets:RawSheetsV5::from_sheets_v4(&request.sheets)};
        let new_problem=serde_json::to_string_pretty(&new_request).unwrap();
        let new_problem_path = root_path
            .join("assets/v5")
            .join(problem_name);  // 确保文件名以 `.json` 结尾
        fs::write(new_problem_path, new_problem).unwrap();
    }
    
}
