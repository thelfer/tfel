include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Exception")

# a simple macro
macro(tests_material test_arg)
  if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.6)
    add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
    add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
    add_dependencies(check ${test_arg})
  else (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.6)
    add_executable(${test_arg} ${test_arg}.cxx)
    add_test(${test_arg} ${EXECUTABLE_OUTPUT_PATH}/${test_arg})
  endif (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.6)
  target_link_libraries(${test_arg} TFELMaterial TFELMath TFELException)
endmacro(tests_material)

tests_material(boundsCheck)

