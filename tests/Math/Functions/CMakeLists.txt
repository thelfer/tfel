include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/tests/Math/Functions/include")
link_directories("${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

# a simple macro
macro(tests_functions test_arg)
  add_executable(${test_arg} ${test_arg}.cxx)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  target_link_libraries(${test_arg}  TFELMath TFELUtilities TFELException)
endmacro(tests_functions)

tests_functions(function)
tests_functions(power)
tests_functions(vector-function)
tests_functions(derivate) 
