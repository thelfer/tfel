add_subdirectory(std)
add_subdirectory(tfel)
add_subdirectory(mfront)
add_subdirectory(mtest)
add_subdirectory(mfm_test_generator)
if(enable-testing)
  add_subdirectory(tests)
endif()

