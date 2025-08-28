#!/bin/bash

SCRIPT_DIR=$(dirname "$(realpath "$0")")
cd "$SCRIPT_DIR"
echo $(pwd)

mkdir sources
mkdir sources/external
cp ./Cmago.toml sources/
cp ../config.json sources/
cp -r ../cl_base sources/
cp -r ../cl_solver sources/
cp -r ../executables sources/
cp -r ../web_api sources/
cp -r ../external/HiGHS sources/external
cp -r ../external/argparse sources/external
cp -r ../external/json sources/external

docker build -f build.dockerfile -t cut_less_build .
rm -rf ./sources