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

if ! test -f "${ARTEFACTS}/libStftPitchShiftPlugin_SharedCode.a"; then
  exit 1
fi

echo "DONE!"
ls -m "${ARTEFACTS}"
