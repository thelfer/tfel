add_subdirectory(mfront-behaviours)
add_subdirectory(mfront-finitestrain)
add_subdirectory(mfront-formation)
add_subdirectory(mfront-formation-tp)
add_subdirectory(mfront-presentation)
add_subdirectory(mtest-presentation)

if(HAVE_PANDOC)
  add_subdirectory(mfront-user-meeting-2015)
  add_subdirectory(portable-behaviour)
  add_subdirectory(Materiaux2014)
endif(HAVE_PANDOC)

add_subdirectory(ClubCast3M2014)
