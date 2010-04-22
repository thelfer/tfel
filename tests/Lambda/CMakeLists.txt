include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Exception"
                 "${PROJECT_BINARY_DIR}/src/Math")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# a simple macro
macro(tests_lambda test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_link_libraries(${test_arg} TFELMath TFELException)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_lambda)

tests_lambda(lambda)
