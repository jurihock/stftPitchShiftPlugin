#!/bin/bash

BASE=$(dirname "$(readlink -f "$0")")
ROOT=$(dirname "${BASE}")

ARGS="--strictness-level 5 --validate-in-process --validate"
CONFIG=Release
INPUT=${ROOT}
OUTPUT=${ROOT}/build

ARTEFACTS=${OUTPUT}/StftPitchShiftPlugin_artefacts/${CONFIG}
PLUGINVAL=${OUTPUT}/_deps/pluginval-src/Contents/MacOS/pluginval

# Execute pluginval.
"${PLUGINVAL}" ${ARGS} "${ARTEFACTS}/VST3/StftPitchShiftPlugin.vst3" || exit $?

# Are you trying to validate a .component on macOS?
# You’ll need to make sure the plugin is picked up by the Audio Unit registry
# by moving or copying it to /Library/Audio/Plug-ins/components (or the ~/Library version).
# You might want to run killall -9 AudioComponentRegistrar to kick the registrar to pick it up.
# Why? Like all JUCE AU hosts, pluginval opens the .plist and gets the component ID out of that to load it up.
# https://melatonin.dev/blog/pluginval-is-a-plugin-devs-best-friend#troubleshooting-pluginval-no-types-found
if [ -d ~/Library/Audio/Plug-Ins/Components ]; then

  echo "Skipping AU plugin validation (issue #5)"
  # cp -rfv "${ARTEFACTS}/AU/StftPitchShiftPlugin.component" ~/Library/Audio/Plug-Ins/Components || exit $?
  # "${PLUGINVAL}" ${ARGS} "${ARTEFACTS}/AU/StftPitchShiftPlugin.component" || exit $?
  # rm -rfv ~/Library/Audio/Plug-Ins/Components/StftPitchShiftPlugin.component || exit $?

fi

# Delete intermediate files.
rm -fv "${ARTEFACTS}/libStftPitchShiftPlugin_SharedCode.a" || exit $?

# Zip binaries to preserve file permissions during artifact upload.
# https://github.com/actions/upload-artifact/issues/38
# https://github.com/actions/upload-artifact#permission-loss
pushd "${ARTEFACTS}"
zip -rm StftPitchShiftPlugin.zip . || exit $?
popd
