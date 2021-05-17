# a simple macro
macro(tests_expr test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT
		 "PATH=$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException TFELTests)
endmacro(tests_expr)

tests_expr(Expr)
tests_expr(Expr2)
tests_expr(Expr3)
tests_expr(Expr4)
tests_expr(Expr5)
tests_expr(Expr6)
tests_expr(Expr7)
tests_expr(Expr8)
tests_expr(Expr9)
tests_expr(test_expr1)
tests_expr(test_expr2)
tests_expr(test_expr6)
tests_expr(test_expr12)

