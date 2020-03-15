# a simple macro
macro(tests_fsalgorithm test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_include_directories(${test_arg}
    PRIVATE ${CMAKE_SOURCE_DIR}/include)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_fsalgorithm)

tests_fsalgorithm(accumulate)
tests_fsalgorithm(equal)
tests_fsalgorithm(for_each)
tests_fsalgorithm(generate)
tests_fsalgorithm(inner_product)
tests_fsalgorithm(iota)
tests_fsalgorithm(test_FSalgorithm)
tests_fsalgorithm(test_FSalgorithm2)
tests_fsalgorithm(transform)
