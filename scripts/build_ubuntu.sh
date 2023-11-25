#!/bin/bash

BASE=$(dirname "$(readlink -f "$0")")
ROOT=$(dirname "${BASE}")

ARGS=
CONFIG=Release
INPUT=${ROOT}
OUTPUT=${ROOT}/build

ARTEFACTS=${OUTPUT}/StftPitchShiftPlugin_artefacts/${CONFIG}

cmake ${ARGS} -DCMAKE_BUILD_TYPE=${CONFIG} -S "${INPUT}" -B "${OUTPUT}" || exit /b
cmake --build "${OUTPUT}" || exit /b

ls -m "${ARTEFACTS}"
