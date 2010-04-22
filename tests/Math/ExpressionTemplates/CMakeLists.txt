include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/tests/Math/ExpressionTemplates/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# a simple macro
macro(tests_expressiontemplates test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_expressiontemplates)

tests_expressiontemplates(test)
tests_expressiontemplates(test2)
tests_expressiontemplates(test3)
tests_expressiontemplates(test4)
tests_expressiontemplates(test5)
tests_expressiontemplates(test6)
tests_expressiontemplates(test7)
tests_expressiontemplates(test8)
tests_expressiontemplates(test9)
tests_expressiontemplates(test10)
tests_expressiontemplates(test11)
tests_expressiontemplates(test12)
tests_expressiontemplates(tvector)
tests_expressiontemplates(tvector2)
