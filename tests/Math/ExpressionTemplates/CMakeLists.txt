include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/tests/Math/ExpressionTemplates/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

# a simple macro
macro(tests_expr test_arg)
  add_executable(${test_arg} ${test_arg}.cxx)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException)
endmacro(tests_expr)

tests_expr(test_expr1)
tests_expr(test_expr2)
tests_expr(test_expr3)
tests_expr(test_expr4)
tests_expr(test_expr5)
tests_expr(test_expr6)
tests_expr(test_expr7)
tests_expr(test_expr8)
tests_expr(test_expr9)
tests_expr(test_expr10)
tests_expr(test_expr11)
tests_expr(test_expr12)
tests_expr(test_expr_tvector)
tests_expr(test_expr_tvector2)
