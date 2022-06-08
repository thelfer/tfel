add_subdirectory(std)
add_subdirectory(tfel)
add_subdirectory(mfront)
add_subdirectory(mtest)
if(enable-testing)
  add_subdirectory(tests)
endif()

