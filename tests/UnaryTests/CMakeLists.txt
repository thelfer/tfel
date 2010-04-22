include_directories("${PROJECT_SOURCE_DIR}/include"
                    "${PROJECT_SOURCE_DIR}/tests/UnaryTests/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")
link_directories("${PROJECT_BINARY_DIR}/src/Exception")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# a simple macro
macro(tests_unary_test test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_link_libraries(${test_arg} TFELMath TFELUtilities TFELException)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_unary_test)

tests_unary_test(CubicRoots)
tests_unary_test(FracSimplify)
tests_unary_test(IntegersSet)
tests_unary_test(MultipleDeque)
tests_unary_test(MultipleVector)
tests_unary_test(concept_check_test)
tests_unary_test(name)
tests_unary_test(promote)
tests_unary_test(qt)
tests_unary_test(qt2)
tests_unary_test(qt3)
tests_unary_test(stensor)
tests_unary_test(tmatrix)
tests_unary_test(tvector)
tests_unary_test(typelist)
tests_unary_test(tuple)
tests_unary_test(gentype)

