include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Tests"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

# a simple macro
macro(tests_st2tost2 test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELPhysicalConstants>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException TFELTests)
endmacro(tests_st2tost2)

tests_st2tost2(st2tost2)
tests_st2tost2(st2tost2-2)
tests_st2tost2(st2tost2-3)
tests_st2tost2(ST2toST2FromTinyMatrixView)
tests_st2tost2(ST2toST2FromTinyMatrixView2)
tests_st2tost2(StensorSquareDerivative) 
tests_st2tost2(ST2toST2ChangeBasis)
tests_st2tost2(ST2toST2PushForward)
tests_st2tost2(ConvertToTangentModuliTest)
tests_st2tost2(ConvertLogarithmicStrainTangentOperator)
tests_st2tost2(ComputeDeterminantSecondDerivativeTest)
tests_st2tost2(ComputeDeviatorDeterminantSecondDerivativeTest)
tests_st2tost2(SymmetricStensorProductDerivativeTest)
