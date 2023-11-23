# https://github.com/jurihock/stftPitchShift

CPMAddPackage(
  NAME stftpitchshift
  VERSION 2.0
  GIT_TAG e92dbd91e2e737ebad38dec25547a5e5e72dc792
  GITHUB_REPOSITORY jurihock/stftPitchShift
  DOWNLOAD_ONLY YES)

if(stftpitchshift_ADDED)

  add_library(stftpitchshift INTERFACE)

  target_include_directories(stftpitchshift
    INTERFACE "${stftpitchshift_SOURCE_DIR}/cpp")

  target_compile_features(stftpitchshift
    INTERFACE cxx_std_20)

endif()
