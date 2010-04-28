include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Tests")

# a simple macro
macro(tests_test test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_link_libraries(${test_arg} TFELTests)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_test)

tests_test(testcase)
tests_test(testsuite)
tests_test(testproxy)
tests_test(testproxy2)
tests_test(testmanager)
tests_test(testfunctionwrapper)
tests_test(stdstreamtestoutput)
tests_test(multipletestoutputs)
