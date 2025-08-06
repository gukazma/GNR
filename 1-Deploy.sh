#!/bin/bash
set -e

conda env create -f environment.yml --prefix ./env
conda activate ./env

mkdir "build"

cd "build"

cmake ..