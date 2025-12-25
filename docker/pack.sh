#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
cd "$SCRIPT_DIR"
echo $(pwd)

export CC=clang
export CXX=clang++
cd ..
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ..
ninja "main_json" -j4
MAIN=main_json
TARGET_DIR="../docker/binaries"
mkdir $TARGET_DIR
cp $MAIN "$TARGET_DIR"
ldd $MAIN | grep "=>" | awk '{print $3}' | while read lib; do
    if [ -f "$lib" ]; then
        cp "$lib" "$TARGET_DIR"
    fi
done
cd ../web_api/server
SERVER=server
cargo build --release --bin $SERVER
cd ..
TARGET_DIR="../docker/binaries"
cp target/release/$SERVER "$TARGET_DIR"
ldd target/release/$SERVER | grep "=>" | awk '{print $3}' | while read lib; do
    if [ -f "$lib" ]; then
        cp "$lib" "$TARGET_DIR"
    fi
done

cd ../docker

docker build -t cut_less .
docker save -o cut_less_image.tar cut_less:latest
# rm -rf ./binaries
cd binaries
echo "start server"
./$SERVER