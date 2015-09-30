include_directories("${CADNA_INCLUDE_PATH}")
include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${CADNA_LIBRARY_PATH}")

macro(tests_math_cadna test_arg)
  add_executable(cadna_${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME cadna_${test_arg} COMMAND cadna_${test_arg})
  add_dependencies(check cadna_${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST cadna_${test_arg}
      PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMath>\;${CADNA_LIBRARY_PATH}/lib\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(cadna_${test_arg} TFELMath TFELTests cadna_cxx)
endmacro(tests_math_cadna)

tests_math_cadna(cadna)
# tests_math_cadna(stensor_eigenvalues)