# https://github.com/ImJimmi/JIVE

CPMAddPackage(
  NAME JIVE
  VERSION 1.0.0
  GIT_TAG 2cadd3a84fe7f502376d18b20087c3d96bbbf0cb
  GITHUB_REPOSITORY jurihock/JIVE
  DOWNLOAD_ONLY YES)

if(JIVE_ADDED)

  add_subdirectory(${JIVE_SOURCE_DIR})

endif()
