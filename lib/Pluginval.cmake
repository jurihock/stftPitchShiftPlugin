# https://github.com/Tracktion/pluginval

set(PLUGINVAL_LATEST_URL  "")

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(PLUGINVAL_LATEST_URL  "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_macOS.zip")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(PLUGINVAL_LATEST_URL  "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_Linux.zip")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(PLUGINVAL_LATEST_URL  "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_Windows.zip")
endif()

CPMAddPackage(
  NAME pluginval
  VERSION latest
  URL ${PLUGINVAL_LATEST_URL})
