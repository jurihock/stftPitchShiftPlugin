#!/bin/bash

BASE=$(dirname "$(readlink -f "$0")")
ROOT=$(dirname "${BASE}")

ARGS="--strictness-level 5 --validate-in-process --validate"
CONFIG=Release
INPUT=${ROOT}
OUTPUT=${ROOT}/build

ARTEFACTS=${OUTPUT}/StftPitchShiftPlugin_artefacts/${CONFIG}
PLUGINVAL=${OUTPUT}/_deps/pluginval-src/pluginval

"${PLUGINVAL}" ${ARGS} "${ARTEFACTS}/VST3/StftPitchShiftPlugin.vst3" || exit $?
