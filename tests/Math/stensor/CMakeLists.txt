include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/tests/Math/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
		 "${PROJECT_BINARY_DIR}/src/Tests")

# a simple macro
macro(tests_math_stensor test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(${test_arg} TFELMath TFELUtilities TFELException TFELTests)
endmacro(tests_math_stensor)

tests_math_stensor(stensor)
tests_math_stensor(stensor2)
tests_math_stensor(stensor4)
tests_math_stensor(stensor5)
tests_math_stensor(stensor6)
tests_math_stensor(stensor7)
tests_math_stensor(stensor8)
tests_math_stensor(stensor9)
tests_math_stensor(stensor10)
tests_math_stensor(stensor_changeBasis)
tests_math_stensor(stensor_changeBasis2)
tests_math_stensor(stensor_changeBasis3)
tests_math_stensor(stensor_changeBasis4)
tests_math_stensor(stensor_eigenvalues)
tests_math_stensor(stensor_eigenvectors)
tests_math_stensor(stensor_eigenvectors2)
tests_math_stensor(stensor_isotropic_function)
tests_math_stensor(StensorFromTinyMatrixColumnView)
