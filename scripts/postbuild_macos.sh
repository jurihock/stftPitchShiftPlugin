#!/bin/bash

BASE=$(dirname "$(readlink -f "$0")")
ROOT=$(dirname "${BASE}")

ARGS="--strictness-level 5 --validate-in-process --validate"
CONFIG=Release
INPUT=${ROOT}
OUTPUT=${ROOT}/build

ARTEFACTS=${OUTPUT}/StftPitchShiftPlugin_artefacts/${CONFIG}
PLUGINVAL=${OUTPUT}/_deps/pluginval-src/Contents/MacOS/pluginval

"${PLUGINVAL}" ${ARGS} "${ARTEFACTS}/VST3/StftPitchShiftPlugin.vst3" || exit $?

# https://melatonin.dev/blog/pluginval-is-a-plugin-devs-best-friend#troubleshooting-pluginval-no-types-found
cp -rf "${ARTEFACTS}/AU/StftPitchShiftPlugin.component" ~/Library/Audio/Plug-Ins/Components || exit $?
"${PLUGINVAL}" ${ARGS} "${ARTEFACTS}/AU/StftPitchShiftPlugin.component" || exit $?
rm -rf ~/Library/Audio/Plug-Ins/Components/StftPitchShiftPlugin.component || exit $?
