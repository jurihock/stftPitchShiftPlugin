# https://github.com/ImJimmi/JIVE

CPMAddPackage(
  NAME JIVE
  VERSION 1.0.0
  GIT_TAG ae85c77b3c216edf4708081cbc0c34110d282597
  GITHUB_REPOSITORY ImJimmi/JIVE
  DOWNLOAD_ONLY YES)

if(JIVE_ADDED)

  add_subdirectory(${JIVE_SOURCE_DIR})

endif()
