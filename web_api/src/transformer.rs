use cl_web_api::part::raw_part_v3::RawPartV3;
use cl_web_api::part::raw_part_v3::RawPartsV3;
use cl_web_api::sheet::raw_sheet_v3::RawSheetsV3;
use cl_web_api::sheet::raw_sheet_v3::RawSheetV3;
use std::env;
use std::fs;
use cl_web_api::request_data::RequestData;
use cl_web_api::request_data::request_data_v3::RequestDataV3;

#[tokio::main]
async fn main() {
    let problems=vec!("test_problem.json","test_problem2.json","test_problem3.json","test_problem4.json");
    let exe_path = env::current_exe().unwrap();
    let root_path = exe_path.parent().unwrap().parent().unwrap().parent().unwrap().parent().unwrap();
    for problem_name in &problems{
        let problem_path = root_path.join("assets").join(problem_name);
        let problem  = fs::read_to_string(problem_path).unwrap();
        let request:RequestData=serde_json::from_str(&problem).unwrap();
        let mut new_request=RequestDataV3{config:request.config.clone(),parts:RawPartsV3{raw_parts:Vec::new()},sheets:RawSheetsV3{raw_sheets:Vec::new()}};
        for part in &request.parts{
            let raw_part=RawPartV3{id:part.id.to_string(),size:part.size.clone(),rotatable:part.rotatable,qty:part.qty};
            new_request.parts.raw_parts.push(raw_part);
        }
        for sheet in &request.sheets{
            let raw_sheet=RawSheetV3{id:sheet.id.to_string(),size:sheet.size.clone(),small:sheet.small,qty:sheet.qty};
            new_request.sheets.raw_sheets.push(raw_sheet);
        }
        let new_problem=serde_json::to_string_pretty(&new_request).unwrap();
        let new_problem_path = root_path
            .join("assets")
            .join("v3")
            .join(problem_name);  // 确保文件名以 `.json` 结尾
        fs::write(new_problem_path, new_problem).unwrap();
    }
    
}
