macro(tests_math_cadna test_arg)
  add_executable(cadna_${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_include_directories(cadna_${test_arg}
    PRIVATE ${CADNA_INCLUDE_PATH})
  target_link_libraries(cadna_${test_arg}
    TFELMath TFELTests ${TFEL_CADNA_LIBRARY})
  add_test(NAME cadna_${test_arg} COMMAND cadna_${test_arg})
  add_dependencies(check cadna_${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST cadna_${test_arg}
      PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMath>\;${CADNA_LIBRARY_PATH}/lib\;$<TARGET_FILE_DIR:TFELPhysicalConstants>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
endmacro(tests_math_cadna)

tests_math_cadna(cadna)
# tests_math_cadna(stensor_eigenvalues)
