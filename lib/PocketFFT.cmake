# https://gitlab.mpcdf.mpg.de/mtr/pocketfft

CPMAddPackage(
  NAME pocketfft
  VERSION 2023.09.25
  GIT_TAG 128707fc745348d9dae5f1e37cd289aa31571dce
  GIT_REPOSITORY https://gitlab.mpcdf.mpg.de/mtr/pocketfft
  DOWNLOAD_ONLY YES)

if(pocketfft_ADDED)

  add_library(pocketfft INTERFACE)

  target_include_directories(pocketfft
    INTERFACE "${pocketfft_SOURCE_DIR}")

  target_compile_definitions(pocketfft
    INTERFACE -DPOCKETFFT_NO_MULTITHREADING)

  target_compile_definitions(pocketfft
    INTERFACE -DPOCKETFFT_CACHE_SIZE=10)

  if(UNIX)
    target_link_libraries(pocketfft
      INTERFACE pthread)
  endif()

endif()
