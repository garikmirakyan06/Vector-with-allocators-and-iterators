#!/bin/bash

ROOT_DIR=$(dirname "$(dirname "$(realpath "$0")")")


cd ${ROOT_DIR}
rm -rf bin 
rm -rf build
mkdir build
cd build
cmake ..
make -j $(nproc)

