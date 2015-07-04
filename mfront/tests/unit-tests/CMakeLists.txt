include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/mfront/include")
link_directories("${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Glossary"
                 "${PROJECT_BINARY_DIR}/src/System"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception"
		 "${PROJECT_BINARY_DIR}/src/Tests"
		 "${PROJECT_BINARY_DIR}/mfront/src")

# a simple macro
macro(tests_mfront test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg}
    TFELMFront TFELMaterial TFELMath
    TFELSystem  TFELGlossary
    TFELUtilities TFELException TFELTests)
endmacro(tests_mfront)

macro(tests_mfront2 test_arg)
  configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/unit-tests/${test_arg}.cxx.in"
    "${PROJECT_BINARY_DIR}/mfront/tests/unit-tests/${test_arg}.cxx" @ONLY)
  add_executable(${test_arg} EXCLUDE_FROM_ALL
    "${PROJECT_BINARY_DIR}/mfront/tests/unit-tests/${test_arg}.cxx")
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg}
    TFELMFront TFELMaterial TFELMath
    TFELSystem  TFELGlossary
    TFELUtilities TFELException TFELTests)
endmacro(tests_mfront2)

tests_mfront(BehaviourDataTest)
tests_mfront(BehaviourDescriptionTest)
tests_mfront(BehaviourBrickProviderTest)
tests_mfront2(MFrontTest)
tests_mfront2(BehaviourBrickRequirementManagerTest)
