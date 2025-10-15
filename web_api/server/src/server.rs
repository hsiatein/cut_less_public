use warp::Filter;
use utils::{handle_request_v4_cli_mt};


#[tokio::main]
async fn main() {
    let process_route = warp::post()
        .and(warp::path("cut_less"))
        .and(warp::body::json())
        .and_then(handle_request_v4_cli_mt);

    warp::serve(process_route)
        .run(([0, 0, 0, 0], 6002))
        .await;
}