juce_add_plugin(${CMAKE_PROJECT_NAME}
  PRODUCT_NAME             "${CMAKE_PROJECT_NAME}"
  VERSION                  "${CMAKE_PROJECT_VERSION}"
  COMPANY_NAME             "Juergen Hock"
  COMPANY_EMAIL            "juergen.hock@jurihock.de"
  COMPANY_WEBSITE          "https://github.com/jurihock/stftPitchShiftPlugin"
  BUNDLE_ID                "de.jurihock.stftpitchshift"
  PLUGIN_MANUFACTURER_CODE Hock
  PLUGIN_CODE              Stft
  IS_SYNTH                 FALSE
  IS_MIDI_EFFECT           FALSE
  NEEDS_MIDI_INPUT         FALSE
  NEEDS_MIDI_OUTPUT        FALSE
  COPY_PLUGIN_AFTER_BUILD  FALSE
  FORMATS                  AU VST3 Standalone)

juce_generate_juce_header(${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/..")

target_sources(${CMAKE_PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/PluginEditor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/PluginProcessor.cpp")

target_compile_definitions(${CMAKE_PROJECT_NAME}
  PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE
    juce::juce_audio_utils
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE
    stftpitchshift)

# FIX
# If you are using Link Time Optimisation (LTO), the new linker introduced in Xcode 15 may produce a broken binary.
# As a workaround, add either '-Wl,-weak_reference_mismatches,weak' or '-Wl,-ld_classic' to your linker flags.
# Once you've selected a workaround, you can add JUCE_SILENCE_XCODE_15_LINKER_WARNING
# to your preprocessor definitions to silence this warning.
# https://forum.juce.com/t/vst-au-builds-fail-after-upgrading-to-xcode-15/57936/43

target_link_options(${CMAKE_PROJECT_NAME}
  PRIVATE
    -Wl,-ld_classic)

target_compile_definitions(${CMAKE_PROJECT_NAME}
  PRIVATE
    JUCE_SILENCE_XCODE_15_LINKER_WARNING)
