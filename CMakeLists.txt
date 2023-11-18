cmake_minimum_required(VERSION 3.1...3.18)

project(StftPitchShiftPlugin VERSION 1.0.0)

if(MSVC)

  # optionally treat warnings as errors
  add_compile_options(/W3 /WX)

else()

  # optionally treat warnings as errors
  add_compile_options(-Wall -Werror)

endif()

include("${CMAKE_CURRENT_LIST_DIR}/lib/CPM.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/lib/JUCE.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/lib/StftPitchShift.cmake")

include("${CMAKE_CURRENT_LIST_DIR}/src/StftPitchShiftPlugin/Plugin.cmake")
