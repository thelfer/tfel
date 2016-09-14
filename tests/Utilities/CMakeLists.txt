include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Utilities")
link_directories("${PROJECT_BINARY_DIR}/src/Tests")

# a simple macro
macro(tests_utilities test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(${test_arg} TFELUtilities TFELTests)
endmacro(tests_utilities)

tests_utilities(gentype)
tests_utilities(gentype2)
tests_utilities(gentype3)
tests_utilities(StringAlgorithms)
tests_utilities(CxxTokenizerTest)
tests_utilities(CxxTokenizerTest2)
tests_utilities(DataTest)

