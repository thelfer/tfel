include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

# a simple macro
macro(tests_st2tost2 test_arg)
  add_executable(${test_arg} ${test_arg}.cxx)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException)
endmacro(tests_st2tost2)

tests_st2tost2(st2tost2)
tests_st2tost2(st2tost2-2)
tests_st2tost2(st2tost2-3)
tests_st2tost2(ST2toST2FTMV) 
