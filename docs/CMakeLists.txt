if(GENERATE_DOXYGEN)
  add_subdirectory(doxygen)
endif(GENERATE_DOXYGEN)

if(GENERATE_REFERENCE_DOC)
  add_subdirectory(tfel)
  add_subdirectory(release-notes-2.0)
  add_subdirectory(release-notes-2.0.3)
  add_subdirectory(release-notes-2.0.4)
  add_subdirectory(release-notes-3.0)
  add_subdirectory(tutorial)
  add_subdirectory(presentations)
endif(GENERATE_REFERENCE_DOC)

if(GENERATE_WEBSITE)
  add_subdirectory(web)
endif(GENERATE_WEBSITE)

add_subdirectory(gallery)
add_subdirectory(mfront)
add_subdirectory(mtest)
add_subdirectory(ptest)

