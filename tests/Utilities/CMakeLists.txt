include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")
link_directories("${PROJECT_BINARY_DIR}/src/Tests")

# a simple macro
macro(tests_utilities test_arg)
  add_executable(${test_arg} ${test_arg}.cxx)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  target_link_libraries(${test_arg} TFELUtilities)
  target_link_libraries(${test_arg} TFELTests)
endmacro(tests_utilities)

tests_utilities(gentype)
tests_utilities(gentype2)
tests_utilities(tuple)
tests_utilities(Name)
tests_utilities(Name2)
tests_utilities(IntegersSet)
tests_utilities(MultipleDeque)
tests_utilities(MultipleVector)

