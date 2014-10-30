if(DOXYGEN_FOUND)
  add_subdirectory(doxygen)
endif(DOXYGEN_FOUND)

if(HAVE_LATEX)
  add_subdirectory(tfel)
  add_subdirectory(release-notes-2.0)
  add_subdirectory(tutorial)
endif(HAVE_LATEX)

add_subdirectory(mfront)
add_subdirectory(mtest)

if(HAVE_LATEX OR HAVE_PANDOC)
  add_subdirectory(presentations)
endif(HAVE_LATEX OR HAVE_PANDOC)

if(HAVE_PANDOC)
  add_subdirectory(web)
endif(HAVE_PANDOC)
