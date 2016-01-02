if(HAVE_CASTEM)
  add_subdirectory(castem)
  add_subdirectory(castem2)
endif(HAVE_CASTEM)

if(HAVE_ASTER)
  add_subdirectory(aster)
endif(HAVE_ASTER)

if(HAVE_ABAQUS)
  add_subdirectory(abaqus)
endif(HAVE_ABAQUS)

if(HAVE_CYRANO)
  add_subdirectory(cyrano)
endif(HAVE_CYRANO)
