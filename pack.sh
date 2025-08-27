#!/bin/bash

TARGET_DIR="binaries"

mkdir -p $TARGET_DIR

cp build/main_json "$TARGET_DIR"
ldd build/main_json | grep "=>" | awk '{print $3}' | while read lib; do
    if [ -f "$lib" ]; then
        cp "$lib" "$TARGET_DIR"
    fi
done

cp web_api/target/debug/server_v2 "$TARGET_DIR"
ldd web_api/target/debug/server_v2 | grep "=>" | awk '{print $3}' | while read lib; do
    if [ -f "$lib" ]; then
        cp "$lib" "$TARGET_DIR"
    fi
done

cp config.json "$TARGET_DIR"

echo "complete"