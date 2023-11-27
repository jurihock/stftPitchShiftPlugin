# https://github.com/jurihock/stftPitchShift

CPMAddPackage(
  NAME stftpitchshift
  VERSION 2.0
  GIT_TAG 00ae8a36af99a58ea0a2c8fac7c87c09a6c9567e
  GITHUB_REPOSITORY jurihock/stftPitchShift
  DOWNLOAD_ONLY YES)

if(stftpitchshift_ADDED)

  add_library(stftpitchshift INTERFACE)

  target_include_directories(stftpitchshift
    INTERFACE "${stftpitchshift_SOURCE_DIR}/cpp")

  target_compile_features(stftpitchshift
    INTERFACE cxx_std_20)

endif()
