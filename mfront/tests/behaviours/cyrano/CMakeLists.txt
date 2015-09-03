include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

set(mfront_tests_SOURCES
  MaterialPropertiesBoundsCheck
  Elasticity
  OrthotropicElastic
  Norton
  ImplicitNorton
  OrthotropicCreep
  EllipticCreep)

if(NOT WIN32)
  set(mfront_tests_SOURCES
      Elasticity2
      Elasticity3
      OrthotropicElastic2
      ${mfront_tests_SOURCES})

mfront_dependencies(MFrontCyranoBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)
endif(NOT WIN32)

mfront_behaviour_check_library(MFrontCyranoBehaviours cyrano  ${mfront_tests_SOURCES})
target_link_libraries(MFrontCyranoBehaviours
  CyranoInterface
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)
if(enable-cxx-11)
  target_link_libraries(MFrontCyranoBehaviours
    MFrontProfiling)
endif(enable-cxx-11)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontCyranoBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

get_property(MFrontCyranoBehavioursBuildPath TARGET MFrontCyranoBehaviours PROPERTY LOCATION)
get_property(MFrontMaterialPropertiesCyranoBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)

macro(test_cyrano test_arg)
  configure_file(cyrano${test_arg}.mtest.in
    cyrano${test_arg}.mtest @ONLY)
  add_test(NAME cyrano${test_arg}_mtest
    COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false cyrano${test_arg}.mtest)
  set_tests_properties(cyrano${test_arg}_mtest
    PROPERTIES DEPENDS MFrontCyranoBehaviours)
  set_tests_properties(cyrano${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
  add_custom_target(cyrano${test_arg}.mtest-install
    ${CMAKE_COMMAND} -E copy cyrano${test_arg}.mtest
    ${CMAKE_INSTALL_PREFIX}/share/mfront/tests/behaviours/cyrano/cyrano${test_arg}.mtest)
  add_dependencies(tests-install
    cyrano${test_arg}.mtest-install)
endmacro(test_cyrano)

test_cyrano(materialpropertiesboundscheck)
test_cyrano(materialpropertiesboundscheck2)
test_cyrano(materialpropertiesboundscheck3)
test_cyrano(materialpropertiesboundscheck4)
test_cyrano(materialpropertiesboundscheck5)
test_cyrano(materialpropertiesboundscheck6)
set_tests_properties(cyranomaterialpropertiesboundscheck6_mtest
  PROPERTIES ENVIRONMENT "CYRANO_OUT_OF_BOUNDS_POLICY=NONE")
test_cyrano(materialpropertiesboundscheck7)
set_tests_properties(cyranomaterialpropertiesboundscheck7_mtest
  PROPERTIES ENVIRONMENT "CYRANO_OUT_OF_BOUNDS_POLICY=WARNING")
test_cyrano(materialpropertiesboundscheck8)
set_tests_properties(cyranomaterialpropertiesboundscheck8_mtest
  PROPERTIES ENVIRONMENT "CYRANO_OUT_OF_BOUNDS_POLICY=STRICT")
set_tests_properties(cyranomaterialpropertiesboundscheck_mtest
  cyranomaterialpropertiesboundscheck2_mtest
  cyranomaterialpropertiesboundscheck5_mtest
  cyranomaterialpropertiesboundscheck8_mtest
  PROPERTIES WILL_FAIL true)

test_cyrano(elasticity)
test_cyrano(elasticity10)
test_cyrano(norton)
test_cyrano(norton2)
test_cyrano(orthotropiccreep)
test_cyrano(ellipticcreep)
test_cyrano(implicitnorton)
test_cyrano(implicitnorton2)
test_cyrano(implicitnorton3)

if(NOT WIN32)
test_cyrano(elasticity18)
test_cyrano(elasticity19)
test_cyrano(elasticity20)
test_cyrano(elasticity21)
endif(NOT WIN32)
