# a simple macro
macro(tests_rungekutta test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_include_directories(${test_arg}
    PRIVATE ${CMAKE_SOURCE_DIR}/tests/Math/RungeKutta/include)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException)
endmacro(tests_rungekutta)

tests_rungekutta(runge_kutta2)
tests_rungekutta(runge_kutta4)
tests_rungekutta(rk42)
tests_rungekutta(rk54)
