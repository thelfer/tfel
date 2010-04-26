include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/tests/Math/ExpressionTemplates/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# a simple macro
macro(tests_expr test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_expr)

tests_expr(test1)
tests_expr(test2)
tests_expr(test3)
tests_expr(test4)
tests_expr(test5)
tests_expr(test6)
tests_expr(test7)
tests_expr(test8)
tests_expr(test9)
tests_expr(test10)
tests_expr(test11)
tests_expr(test12)
tests_expr(tvector)
tests_expr(tvector2)
