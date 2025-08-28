#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
cd "$SCRIPT_DIR"
echo $(pwd)

CUT_LESS_NAME="main_json"

cd ..
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ..
ninja $CUT_LESS_NAME -j4
cd ..

TARGET_DIR="docker/binaries"
mkdir -p $TARGET_DIR
CUT_LESS_PATH="build/$CUT_LESS_NAME"
cp "$CUT_LESS_PATH" "$TARGET_DIR"
ldd "$CUT_LESS_PATH" | grep "=>" | awk '{print $3}' | while read lib; do
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