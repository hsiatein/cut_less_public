#!/bin/bash
SCRIPT_DIR=$(dirname "$(realpath "$0")")
cd "$SCRIPT_DIR"
echo $(pwd)

if ! docker image inspect alpine_base:latest > /dev/null 2>&1; then
    echo "构建 alpine_base"
    docker build -f alpine_base.dockerfile -t alpine_base .
fi

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
cp -r ../external/googletest sources/external

docker build --progress=plain -f alpine.dockerfile -t cut_less_alpine .
rm -rf ./sources