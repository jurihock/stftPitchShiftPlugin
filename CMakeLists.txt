cmake_minimum_required(VERSION 3.1...3.18)

project(StftPitchShiftPlugin VERSION 1.0.0)

include("${CMAKE_CURRENT_LIST_DIR}/lib/CPM.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/lib/JUCE.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/lib/StftPitchShift.cmake")

include("${CMAKE_CURRENT_LIST_DIR}/src/StftPitchShiftPlugin/Plugin.cmake")
