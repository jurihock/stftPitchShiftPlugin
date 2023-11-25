# https://github.com/jurihock/stftPitchShift

CPMAddPackage(
  NAME stftpitchshift
  VERSION 2.0
  GIT_TAG 5ce0fcc7c5eb7aa13d3f38fde371092b26bbe46f
  GITHUB_REPOSITORY jurihock/stftPitchShift
  DOWNLOAD_ONLY YES)

if(stftpitchshift_ADDED)

  add_library(stftpitchshift INTERFACE)

  target_include_directories(stftpitchshift
    INTERFACE "${stftpitchshift_SOURCE_DIR}/cpp")

  target_compile_features(stftpitchshift
    INTERFACE cxx_std_20)

endif()
