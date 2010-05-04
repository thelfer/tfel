include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")

# a simple macro
macro(tests_utilities test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_link_libraries(${test_arg} TFELUtilities)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_utilities)

tests_utilities(gentype)
tests_utilities(gentype2)
tests_utilities(tuple)
tests_utilities(Name)
tests_utilities(Name2)
tests_utilities(IntegersSet)
tests_utilities(MultipleDeque)
tests_utilities(MultipleVector)

