function(tests_math_numerical_integration test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_include_directories(${test_arg}
    PRIVATE ${PROJECT_SOURCE_DIR}/tests/Math/include/)
  target_link_libraries(${test_arg}
    TFELMath TFELUtilities TFELException TFELTests)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
endfunction(tests_math_numerical_integration)

tests_math_numerical_integration(GaussKronrodQuadratureTest)