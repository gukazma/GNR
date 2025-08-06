#!/bin/bash
set -e

mkdir -p "build"

cd "build"
cmake ..
cmake --build . --config Release -j $(nproc)
cmake --build . --config Debug -j $(nproc)
cd ..