add_subdirectory(tfel)
add_subdirectory(mfront)
add_subdirectory(mtest)
add_subdirectory(mfm_test_generator)
if(enable-testing)
  add_subdirectory(tests)
endif()

# The generation of the stubs of the python modules using pybind11-stubgen is
# deferred: see the end of the main CMakeLists.txt, where the
# generate_python_stubs function is called.
