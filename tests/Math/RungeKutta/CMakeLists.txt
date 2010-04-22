include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/tests/Math/Rungekutta/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# a simple macro
macro(tests_rungekutta test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_rungekutta)

tests_rungekutta(runge_kutta2)
tests_rungekutta(runge_kutta4)
tests_rungekutta(rk42)
tests_rungekutta(rk54)
