include_directories("${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/tfel-check/include")
link_directories("${PROJECT_BINARY_DIR}/tfel-check/src"
  "${PROJECT_BINARY_DIR}/src/Utilities"
  "${PROJECT_BINARY_DIR}/src/System"
  "${PROJECT_BINARY_DIR}/src/Math")


# a simple macro
add_executable(tests_check EXCLUDE_FROM_ALL tests.cxx
  test_Column.cxx
  test_LinearInterpolation.cxx
  test_SplineLocalInterpolation.cxx
  test_SplineInterpolation.cxx
  test_Test.cxx)
add_test(NAME tests_check COMMAND tests_check)
add_dependencies(check tests_check)
if((CMAKE_HOST_WIN32) AND (NOT MSYS))
  set_property(TEST tests_check
               PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELCheck>\;$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
target_link_libraries(tests_check TFELCheck
  TFELMathCubicSpline TFELSystem TFELUtilities TFELTests)

