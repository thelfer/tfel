include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
                 "${PROJECT_BINARY_DIR}/src/Tests")

# a simple macro
macro(tests_expr test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException TFELTests)
endmacro(tests_expr)

tests_expr(test_expr1)
tests_expr(test_expr2)
tests_expr(test_expr6)
tests_expr(test_expr7)
tests_expr(test_expr9)
tests_expr(test_expr12)

