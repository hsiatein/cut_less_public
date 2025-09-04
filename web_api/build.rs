use std::env;

fn main() {
    let build_dir=env::current_dir().unwrap().parent().unwrap().join("build");

    if cfg!(target_os = "linux") && cfg!(target_env = "musl") {
        println!("cargo:warning=build on MUSL");
        let library_path = build_dir.join("cl_solver").to_string_lossy().into_owned();
        println!("cargo:rerun-if-changed={}", library_path);
        println!("cargo:rustc-link-search=native={}", library_path);
        println!("cargo:rustc-link-lib=static=cl_solver");

        let library_path = build_dir.join("lib").to_string_lossy().into_owned();
        println!("cargo:rustc-link-search=native={}", library_path);
        println!("cargo:rustc-link-lib=static=highs");

        let library_path = build_dir.join("cl_base").to_string_lossy().into_owned();
        println!("cargo:rerun-if-changed={}", library_path);
        println!("cargo:rustc-link-search=native={}", library_path);
        println!("cargo:rustc-link-lib=static=cl_base");

        let library_path="/usr/lib";
        println!("cargo:rustc-link-search=native={}", library_path);
        println!("cargo:rustc-link-lib=static=stdc++");
        println!("cargo:rustc-link-lib=static=z");
        println!("cargo:rustc-link-lib=static=c");
        println!("cargo:rustc-link-lib=static=m");
    }
    else{
        println!("cargo:warning=build on GNU");
        let library_path = build_dir.join("cl_solver").to_string_lossy().into_owned();
        println!("cargo:rerun-if-changed={}", library_path);
        println!("cargo:rustc-link-search=native={}", library_path);
        println!("cargo:rustc-link-arg=-Wl,-rpath={}", library_path);
        println!("cargo:rustc-link-lib=dylib=cl_solver");
    }

}