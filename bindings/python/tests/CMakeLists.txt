macro(test_pymfront test)
  configure_file(pymfront${test}.py.in
    pymfront${test}.py @ONLY)
  add_test(NAME pymfront${test}_py
    COMMAND ${PYTHON_EXECUTABLE} pymfront${test}.py)
  if(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(pymfront${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python;$<TARGET_FILE_DIR:py_std__stl_vector>;$<TARGET_FILE_DIR:py_tfel_tests;$<TARGET_FILE_DIR:py_mfront__mfront>")
  else(CMAKE_CONFIGURATION_TYPES)
    set_tests_properties(pymfront${test}_py
      PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python")
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_pymfront)

test_pymfront(test1)
test_pymfront(test2)
