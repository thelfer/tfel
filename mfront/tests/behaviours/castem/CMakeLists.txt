include_directories(
  "${CASTEMHOME}/include"
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

set(mfront_tests_SOURCES
  Elasticity Plasticity
  Norton DDIF2
  Norton_Euler Norton_rk2
  Norton_rk4   Norton_rk42
  Norton_rkCastem
  ImplicitNorton
  ImplicitNorton2
  OrthotropicCreep
  OrthotropicCreep_rk42
  OrthotropicCreep_rkCastem
  ImplicitOrthotropicCreep
  OrthotropicElastic
  StrainHardeningCreep
  Mazars
  Chaboche
  Chaboche2)
  
mfront_behaviour_check_library(MFrontCastemBehaviours umat  ${mfront_tests_SOURCES})
target_link_libraries(MFrontCastemBehaviours
  UMATInterface
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

macro(test_umat test_arg)
  configure_file(${test_arg}.mtest.in
    ${test_arg}.mtest @ONLY)
  add_test(castem${test_arg}_mtest
    ${PROJECT_BINARY_DIR}/mfront/mtest/mtest --verbose=level0 ${test_arg}.mtest)
  set_tests_properties(castem${test_arg}_mtest
    PROPERTIES DEPENDS MFrontCastemBehaviours)
  set_tests_properties(castem${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
endmacro(test_umat)

test_umat(castemelasticity)
test_umat(castemelasticity2)
if(HAVE_CASTEM)
  test_umat(castemelasticity3)
  set_tests_properties(castemelasticity3_mtest
    PROPERTIES DEPENDS MFrontMaterialProperties-castem)
endif(HAVE_CASTEM)
test_umat(castemelasticity4)
test_umat(castemelasticity5)
test_umat(castemelasticity6)
test_umat(castemelasticity7)
test_umat(castemelasticity8)
test_umat(castemelasticity9)
test_umat(castemelasticity10)
test_umat(castemelasticity11)
test_umat(castemelasticity12)
test_umat(castemelasticity13)
test_umat(castemelasticity14)
test_umat(castemelasticity15)
test_umat(castemelasticity16)
test_umat(castemmazars)
test_umat(castemnorton)
test_umat(castemnorton2)
test_umat(castemnorton-euler)
test_umat(castemnorton-rk2)
test_umat(castemnorton-rk4)
test_umat(castemorthotropiccreep)
test_umat(castemorthotropiccreep-rk42)
test_umat(castemorthotropiccreep-rkCastem)
test_umat(castemstrainhardeningcreep)
test_umat(castemplasticity)
test_umat(castemchaboche)
test_umat(castemchaboche2)
