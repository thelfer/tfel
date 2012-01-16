include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/tests/Math/RungeKutta/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

# a simple macro
macro(tests_rungekutta test_arg)
  add_executable(${test_arg} ${test_arg}.cxx)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException)
endmacro(tests_rungekutta)

tests_rungekutta(runge_kutta2)
tests_rungekutta(runge_kutta4)
tests_rungekutta(rk42)
tests_rungekutta(rk54)
