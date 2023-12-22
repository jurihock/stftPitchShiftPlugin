# https://github.com/ImJimmi/JIVE

CPMAddPackage(
  NAME JIVE
  VERSION 1.0.0
  GIT_TAG b65bd837e09a14a7e869d12886a69611a9850e2e
  GITHUB_REPOSITORY ImJimmi/JIVE
  DOWNLOAD_ONLY YES)

if(JIVE_ADDED)

  add_subdirectory(${JIVE_SOURCE_DIR})

endif()
