include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Exception")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# a simple macro
macro(tests_material test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  target_link_libraries(${test_arg} TFELMaterial TFELMath TFELException)
  add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  add_dependencies(check ${test_arg})
endmacro(tests_material)

tests_material(boundsCheck)

