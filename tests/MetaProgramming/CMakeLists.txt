include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# a simple macro
macro(tests_metaprogramming test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_link_libraries(${test_arg} TFELUtilities)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_metaprogramming)

tests_metaprogramming(ConceptCheck)
tests_metaprogramming(HasConstIterator)
tests_metaprogramming(RemoveConstness)
tests_metaprogramming(typelist)
tests_metaprogramming(typelist2)
