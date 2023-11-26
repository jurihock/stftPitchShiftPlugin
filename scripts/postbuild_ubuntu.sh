#!/bin/bash

BASE=$(dirname "$(readlink -f "$0")")
ROOT=$(dirname "${BASE}")

ARGS="--strictness-level 5 --validate-in-process --validate"
CONFIG=Release
INPUT=${ROOT}
OUTPUT=${ROOT}/build

ARTEFACTS=${OUTPUT}/StftPitchShiftPlugin_artefacts/${CONFIG}
PLUGINVAL=${OUTPUT}/_deps/pluginval-src/pluginval

# If specified, avoids tests that create GUI windows,
# which can cause problems on headless CI systems.
# https://github.com/Tracktion/pluginval/blob/develop/docs/Command%20line%20options.md
export SKIP_GUI_TESTS=1

"${PLUGINVAL}" ${ARGS} "${ARTEFACTS}/VST3/StftPitchShiftPlugin.vst3" || exit $?
