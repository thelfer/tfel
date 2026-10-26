add_subdirectory(tfel)
add_subdirectory(mfront)
add_subdirectory(mtest)
add_subdirectory(mfm_test_generator)
if(enable-testing)
  add_subdirectory(tests)
endif()

# Generation of the stubs of the python modules using pybind11-stubgen.
# This call shall be placed after all the calls to add_subdirectory as it
# requires that all the python modules have been declared.
generate_python_stubs()
