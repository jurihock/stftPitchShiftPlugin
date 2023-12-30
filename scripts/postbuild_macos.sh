#!/bin/bash

BASE=$(dirname "$(readlink -f "$0")")
ROOT=$(dirname "${BASE}")

ARGS="--strictness-level 5 --validate-in-process --validate"
CONFIG=Release
PLUGIN=StftPitchShiftPlugin
INPUT=${ROOT}
OUTPUT=${ROOT}/build

ARTEFACTS=${OUTPUT}/${PLUGIN}_artefacts/${CONFIG}
PLUGINVAL=${OUTPUT}/_deps/pluginval-src/Contents/MacOS/pluginval

# Execute pluginval.
"${PLUGINVAL}" ${ARGS} "${ARTEFACTS}/VST3/${PLUGIN}.vst3" || exit $?

# Are you trying to validate a .component on macOS?
# You’ll need to make sure the plugin is picked up by the Audio Unit registry
# by moving or copying it to /Library/Audio/Plug-ins/components (or the ~/Library version).
# You might want to run killall -9 AudioComponentRegistrar to kick the registrar to pick it up.
# Why? Like all JUCE AU hosts, pluginval opens the .plist and gets the component ID out of that to load it up.
# https://melatonin.dev/blog/pluginval-is-a-plugin-devs-best-friend#troubleshooting-pluginval-no-types-found
if [ -d ~/Library/Audio/Plug-Ins/Components ]; then

  cp -rfv "${ARTEFACTS}/AU/${PLUGIN}.component" ~/Library/Audio/Plug-Ins/Components || exit $?

  "${PLUGINVAL}" ${ARGS} "${ARTEFACTS}/AU/${PLUGIN}.component" || exit $?

  rm -rfv ~/Library/Audio/Plug-Ins/Components/${PLUGIN}.component || exit $?

fi

# Delete intermediate files.
rm -fv "${ARTEFACTS}/lib${PLUGIN}_SharedCode.a" || exit $?

# Zip binaries to preserve file permissions during artifact upload.
# https://github.com/actions/upload-artifact/issues/38
# https://github.com/actions/upload-artifact#permission-loss
pushd "${ARTEFACTS}"
zip -rm ${PLUGIN}.zip . || exit $?
popd
