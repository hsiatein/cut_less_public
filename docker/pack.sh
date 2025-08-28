#!/bin/bash
cd ..
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ..
ninja -j4
cd ..

TARGET_DIR="docker/binaries"
mkdir -p $TARGET_DIR
cut_less_exe="build/main_json"
cp "$cut_less_exe" "$TARGET_DIR"
ldd "$cut_less_exe" | grep "=>" | awk '{print $3}' | while read lib; do
    if [ -f "$lib" ]; then
        cp "$lib" "$TARGET_DIR"
    fi
done

cd web_api
cargo build --release
cd ..
cp web_api/target/release/server_v2 "$TARGET_DIR"
ldd web_api/target/release/server_v2 | grep "=>" | awk '{print $3}' | while read lib; do
    if [ -f "$lib" ]; then
        cp "$lib" "$TARGET_DIR"
    fi
done

cp config.json "$TARGET_DIR"

cd docker

docker build -t cut_less .
docker save -o cut_less_image.tar cut_less:latest
rm -rf ./binaries