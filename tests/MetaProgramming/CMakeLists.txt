# a simple macro
macro(tests_metaprogramming test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(${test_arg} TFELUtilities TFELTests)
endmacro(tests_metaprogramming)

tests_metaprogramming(ResultOf)
tests_metaprogramming(IsConstCallable)
tests_metaprogramming(HasConstIterator)
tests_metaprogramming(typelist2)
