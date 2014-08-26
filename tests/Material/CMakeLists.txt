include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
                 "${PROJECT_BINARY_DIR}/src/Tests")

# a simple macro
macro(tests_material test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg} TFELMaterial TFELMath
    TFELUtilities TFELException TFELTests)
endmacro(tests_material)

tests_material(boundsCheck)
tests_material(FiniteStrainBehaviourTangentOperator)

