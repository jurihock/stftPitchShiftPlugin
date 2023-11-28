# https://github.com/jurihock/stftPitchShift

CPMAddPackage(
  NAME stftpitchshift
  VERSION 2.0
  GIT_TAG ef3311ea98ff0ad0689c8becd7c956620830bd27
  GITHUB_REPOSITORY jurihock/stftPitchShift
  DOWNLOAD_ONLY YES)

if(stftpitchshift_ADDED)

  add_library(stftpitchshift INTERFACE)

  target_include_directories(stftpitchshift
    INTERFACE "${stftpitchshift_SOURCE_DIR}/cpp")

  target_compile_features(stftpitchshift
    INTERFACE cxx_std_20)

endif()
