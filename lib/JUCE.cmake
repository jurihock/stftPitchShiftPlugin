# https://github.com/juce-framework/JUCE

CPMAddPackage(
  NAME JUCE
  VERSION 7.0.9
  GIT_TAG 7.0.9
  GITHUB_REPOSITORY juce-framework/JUCE
  DOWNLOAD_ONLY YES)

if(JUCE_ADDED)

  add_subdirectory(${JUCE_SOURCE_DIR})

endif()
