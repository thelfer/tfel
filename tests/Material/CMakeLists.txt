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
  if((CMAKE_HOST_WIN32) AND (NOT MSYS))
    set_property(TEST ${test_arg}
  	         PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMath>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$ENV{PATH}")
  endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(${test_arg} TFELMaterial TFELMath
    TFELUtilities TFELException TFELTests)
endmacro(tests_material)

tests_material(boundsCheck)
tests_material(OrthotropicAxesConventionTest)
tests_material(AbaqusTangentOperator)
tests_material(AbaqusTangentOperator2)
tests_material(FiniteStrainBehaviourTangentOperator)
tests_material(FiniteStrainBehaviourTangentOperator2)
tests_material(FiniteStrainBehaviourTangentOperator3)
tests_material(FiniteStrainBehaviourTangentOperator4)
tests_material(FiniteStrainBehaviourTangentOperator5)
tests_material(FiniteStrainBehaviourTangentOperator6)
tests_material(FiniteStrainBehaviourTangentOperator7)
tests_material(FiniteStrainBehaviourTangentOperator8)
tests_material(FiniteStrainBehaviourTangentOperator9)
tests_material(FiniteStrainBehaviourTangentOperator10)
tests_material(FiniteStrainBehaviourTangentOperator11)

