include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Exception")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# a simple macro
macro(tests_system test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_link_libraries(${test_arg} TFELSystem TFELException)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_system)

tests_system(process_test_target)
tests_system(process)
tests_system(wstream)
tests_system(rstream)
