set(FORMATS Standalone VST3)

if(APPLE)
  list(APPEND FORMATS AU)
endif()

if(LINUX)
  list(APPEND FORMATS LV2)
endif()

juce_add_plugin(${CMAKE_PROJECT_NAME}
  PLUGIN_NAME                   "${CMAKE_PROJECT_NAME} v${CMAKE_PROJECT_VERSION}"
  DESCRIPTION                   "Real-time poly pitch and timbre shifting plugin"
  PRODUCT_NAME                  "${CMAKE_PROJECT_NAME}"
  VERSION                       "${CMAKE_PROJECT_VERSION}"
  COMPANY_NAME                  "jurihock"
  COMPANY_EMAIL                 "juergen.hock@jurihock.de"
  COMPANY_WEBSITE               "https://github.com/jurihock/stftPitchShiftPlugin"
  BUNDLE_ID                     "de.jurihock.stftpitchshift"
  PLUGIN_MANUFACTURER_CODE      "Hock"
  PLUGIN_CODE                   "Stft"
  IS_SYNTH                      FALSE
  IS_MIDI_EFFECT                FALSE
  NEEDS_MIDI_INPUT              FALSE
  NEEDS_MIDI_OUTPUT             FALSE
  MICROPHONE_PERMISSION_ENABLED TRUE
  COPY_PLUGIN_AFTER_BUILD       FALSE
  FORMATS                       ${FORMATS})

juce_generate_juce_header(${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME}
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/..")

file(
  GLOB_RECURSE
    HDR "${CMAKE_CURRENT_LIST_DIR}/*.h"
    CPP "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

target_sources(${CMAKE_PROJECT_NAME}
  PRIVATE
    ${HDR}
    ${CPP})

target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE
    pocketfft
    stftpitchshift)

target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE
    juce::juce_audio_utils
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags)

target_compile_definitions(${CMAKE_PROJECT_NAME}
  PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    DONT_SET_USING_JUCE_NAMESPACE=1)

if(FASTMATH)

  message(STATUS "Enabling fast math")

  if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME}
      PRIVATE
        /fp:fast)
  else()
    target_compile_options(${CMAKE_PROJECT_NAME}
      PRIVATE
        -ffast-math)
  endif()

endif()

if(FASTATAN)

  message(STATUS "Defining ENABLE_ARCTANGENT_APPROXIMATION")

  target_compile_definitions(${CMAKE_PROJECT_NAME}
    PRIVATE
      -DENABLE_ARCTANGENT_APPROXIMATION)

endif()

if(FASTFFT)

  message(STATUS "Defining ENABLE_POCKET_FFT")

  target_compile_definitions(${CMAKE_PROJECT_NAME}
    PRIVATE
      -DENABLE_POCKET_FFT)

endif()

if(LOGGING)

  message(STATUS "Defining ENABLE_PLUGIN_LOGGER")

  target_compile_definitions(${CMAKE_PROJECT_NAME}
    PRIVATE
      -DENABLE_PLUGIN_LOGGER)

endif()

if(CHRONO)

  message(STATUS "Defining ENABLE_PLUGIN_CHRONOMETER")

  target_compile_definitions(${CMAKE_PROJECT_NAME}
    PRIVATE
      -DENABLE_PLUGIN_CHRONOMETER)

endif()

if(WARNINGS)

  message(STATUS "Enabling all warnings")

  if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME}
      PRIVATE
        /W3 /WX)
  else()
    target_compile_options(${CMAKE_PROJECT_NAME}
      PRIVATE
        -Wall -Werror)
  endif()

endif()

if(APPLE)

  # FIX
  # If you are using Link Time Optimisation (LTO), the new linker introduced in Xcode 15 may produce a broken binary.
  # As a workaround, add either '-Wl,-weak_reference_mismatches,weak' or '-Wl,-ld_classic' to your linker flags.
  # Once you've selected a workaround, you can add JUCE_SILENCE_XCODE_15_LINKER_WARNING
  # to your preprocessor definitions to silence this warning.
  # https://forum.juce.com/t/vst-au-builds-fail-after-upgrading-to-xcode-15/57936/43

  message(STATUS "Applying -Wl -ld_classic JUCE_SILENCE_XCODE_15_LINKER_WARNING")

  target_link_options(${CMAKE_PROJECT_NAME}
    PRIVATE
      -Wl,-ld_classic)

  target_compile_definitions(${CMAKE_PROJECT_NAME}
    PRIVATE
      JUCE_SILENCE_XCODE_15_LINKER_WARNING)

endif()
