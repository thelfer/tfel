include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Exception")

# a simple macro
macro(tests_system test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg} TFELSystem TFELException)
endmacro(tests_system)

if(UNIX)
tests_system(process_test_target)
tests_system(process)
tests_system(wstream)
tests_system(rstream)
endif(UNIX)
