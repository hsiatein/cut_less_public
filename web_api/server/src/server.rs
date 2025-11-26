use warp::Filter;
use utils::{handle_request_v4_cli_mt, handle_request_v4_cli_mt_multi};


#[tokio::main]
async fn main() {
    let process_route = warp::post()
        .and(warp::path("cut_less"))
        .and(warp::body::json())
        .and_then(handle_request_v4_cli_mt);

    let process_route_multi = warp::post()
        .and(warp::path("cut_less_multi"))
        .and(warp::body::json())
        .and_then(handle_request_v4_cli_mt_multi);

    let routes = process_route.or(process_route_multi);

    warp::serve(routes)
        .run(([0, 0, 0, 0], 6002))
        .await;
}
