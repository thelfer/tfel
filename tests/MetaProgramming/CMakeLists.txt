include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")

# a simple macro
macro(tests_metaprogramming test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg} TFELUtilities)
endmacro(tests_metaprogramming)

tests_metaprogramming(HasConstIterator)
tests_metaprogramming(typelist)
tests_metaprogramming(typelist2)
