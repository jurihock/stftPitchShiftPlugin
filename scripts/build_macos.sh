#!/bin/bash

BASE=$(dirname "$(readlink -f "$0")")
ROOT=$(dirname "${BASE}")

ARGS=-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
CONFIG=Release
PLUGIN=StftPitchShiftPlugin
INPUT=${ROOT}
OUTPUT=${ROOT}/build

ARTEFACTS=${OUTPUT}/${PLUGIN}_artefacts/${CONFIG}

cmake ${ARGS} -DCMAKE_BUILD_TYPE=${CONFIG} -S "${INPUT}" -B "${OUTPUT}" || exit $?
cmake --build "${OUTPUT}" || exit $?

echo "DONE!"
ls -m "${ARTEFACTS}"
