# https://gitlab.mpcdf.mpg.de/mtr/pocketfft

CPMAddPackage(
  NAME pocketfft
  VERSION 2026.06.30
  GIT_TAG c90e55b3d529f8efa40ed01a20de22405f45fc65
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
