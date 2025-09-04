use std::env;

fn main() {
    
    let library_path="/usr/lib";
    println!("cargo:rerun-if-changed={}", library_path);
    println!("cargo:rustc-link-search=native={}", library_path);
    println!("cargo:rustc-link-arg=-Wl,-rpath={}", library_path);
    println!("cargo:rustc-link-lib=dylib=stdc++");
    let target_dir = env::current_dir().unwrap().parent().unwrap().join("build/cl_solver");    
    let library_path = target_dir.to_str().unwrap().to_string();
    println!("cargo:rerun-if-changed={}", library_path);
    println!("cargo:rustc-link-search=native={}", library_path);
    println!("cargo:rustc-link-arg=-Wl,-rpath={}", library_path);
    println!("cargo:rustc-link-lib=dylib=cl_solver");
}