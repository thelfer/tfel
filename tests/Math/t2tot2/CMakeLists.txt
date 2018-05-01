include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Tests"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

# a simple macro
macro(tests_t2tot2 test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELPhysicalConstants>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException TFELTests)
endmacro(tests_t2tot2)

tests_t2tot2(t2tot2)
tests_t2tot2(t2tot2-specialvalues)
tests_t2tot2(TensorProductDerivative)
tests_t2tot2(TensorSquareDerivative)
tests_t2tot2(TransposeDerivative)
tests_t2tot2(VelocityGradientDerivative)
tests_t2tot2(SpinRateDerivative)
tests_t2tot2(ComputeTensorDeterminantSecondDerivativeTest)

