pub mod part;
pub mod sheet;
pub mod config;
pub mod request_data;
pub mod response;

static FACTOR:f64=10000.;


#[derive(Debug)]
pub struct CliError(pub String);
impl warp::reject::Reject for CliError {
    
}
