#!/bin/bash

BASE=$(dirname "$(readlink -f "$0")")
ROOT=$(dirname "${BASE}")

ARGS="--strictness-level 5 --validate-in-process --validate"
CONFIG=Release
PLUGIN=StftPitchShiftPlugin
INPUT=${ROOT}
OUTPUT=${ROOT}/build

ARTEFACTS=${OUTPUT}/${PLUGIN}_artefacts/${CONFIG}
PLUGINVAL=${OUTPUT}/_deps/pluginval-src/pluginval

# If specified, avoids tests that create GUI windows,
# which can cause problems on headless CI systems.
# https://github.com/Tracktion/pluginval/blob/develop/docs/Command%20line%20options.md
export SKIP_GUI_TESTS=1

# Execute pluginval.
"${PLUGINVAL}" ${ARGS} "${ARTEFACTS}/VST3/${PLUGIN}.vst3" || exit $?

# Delete intermediate files.
rm -fv "${ARTEFACTS}/lib${PLUGIN}_SharedCode.a" || exit $?

# Zip binaries to preserve file permissions during artifact upload.
# https://github.com/actions/upload-artifact/issues/38
# https://github.com/actions/upload-artifact#permission-loss
pushd "${ARTEFACTS}"
zip -rm ${PLUGIN}.zip . || exit $?
popd
