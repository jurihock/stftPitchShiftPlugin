#!/bin/bash

BASE = $(dirname "$(readlink -f "$0")")
ROOT = $(dirname "${BASE}")

CONFIG = Release
INPUT  = ${ROOT}
OUTPUT = ${ROOT}/build

cmake -DCMAKE_BUILD_TYPE=${CONFIG} -S "${INPUT}" -B "${OUTPUT}" || exit /b
cmake --build "${OUTPUT}" || exit /b
