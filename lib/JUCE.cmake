# https://github.com/juce-framework/JUCE

CPMAddPackage(
  NAME JUCE
  VERSION 8.0.14
  GIT_TAG 8.0.14
  GITHUB_REPOSITORY juce-framework/JUCE
  DOWNLOAD_ONLY YES)

if(JUCE_ADDED)

  add_subdirectory(${JUCE_SOURCE_DIR})

endif()
