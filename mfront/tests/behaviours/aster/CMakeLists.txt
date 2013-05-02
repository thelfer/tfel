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
  Elasticity
  OrthotropicElastic
  OrthotropicCreep
  OrthotropicCreep_rk42
  OrthotropicCreep_rkCastem
  ImplicitOrthotropicCreep
  Plasticity
  Norton NortonRK DDIF2
  Norton_Euler Norton_rk2
  Norton_rk4   Norton_rk42
  Norton_rkCastem
  ImplicitNorton
  ImplicitNorton2
  StrainHardeningCreep
  Mazars
  Chaboche
  Chaboche2)

mfront_behaviour_check_library(MFrontAsterBehaviours  aster ${mfront_tests_SOURCES})
target_link_libraries(MFrontAsterBehaviours
  AsterInterface
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

macro(test_aster test_arg)
  configure_file(${test_arg}.mtest.in
    ${test_arg}.mtest @ONLY)
  add_test(aster${test_arg}_mtest 
    ${PROJECT_BINARY_DIR}/mfront/mtest/mtest --verbose=level0 ${test_arg}.mtest)
  set_tests_properties(aster${test_arg}_mtest
    PROPERTIES DEPENDS MFrontAsterBehaviours)
  set_tests_properties(aster${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
endmacro(test_aster)

test_aster(asterelasticity)
test_aster(asterelasticity2)
if(HAVE_CASTEM)
  test_aster(asterelasticity3)
  set_tests_properties(asterelasticity3_mtest
    PROPERTIES DEPENDS MFrontMaterialProperties-castem)
endif(HAVE_CASTEM)
test_aster(asterelasticity4)
test_aster(asterelasticity5)
test_aster(asterelasticity6)
test_aster(asterelasticity7)
test_aster(asterelasticity8)
test_aster(asterelasticity9)
test_aster(asterelasticity10)
test_aster(asterelasticity11)
test_aster(asterelasticity12)
test_aster(asterelasticity13)
test_aster(asterelasticity14)
test_aster(asterelasticity15)
test_aster(asterelasticity16)
test_aster(astermazars)
test_aster(asternorton)
test_aster(asternorton2)
# test_aster(asterorthotropiccreep)
# test_aster(asterorthotropiccreep-rk42)
# test_aster(asterorthotropiccreep-rkCastem)
test_aster(asterimplicitnorton)
test_aster(asterimplicitnorton2)
test_aster(asterimplicitorthotropiccreep)
test_aster(asterplasticity)
test_aster(asterstrainhardeningcreep)
test_aster(asterchaboche)
test_aster(asterchaboche2)
