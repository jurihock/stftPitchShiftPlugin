#!/bin/bash

BASE=$(dirname "$(readlink -f "$0")")
ROOT=$(dirname "${BASE}")

ARGS=-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
CONFIG=Release
INPUT=${ROOT}
OUTPUT=${ROOT}/build

ARTEFACTS=${OUTPUT}/StftPitchShiftPlugin_artefacts/${CONFIG}

cmake ${ARGS} -DCMAKE_BUILD_TYPE=${CONFIG} -S "${INPUT}" -B "${OUTPUT}" || exit $?
cmake --build "${OUTPUT}" || exit $?

chmod +x "${ARTEFACTS}/Standalone/StftPitchShiftPlugin.app/Contents/MacOS/StftPitchShiftPlugin"

ls -m "${ARTEFACTS}"
