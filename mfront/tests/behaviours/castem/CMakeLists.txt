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
  ImplicitNorton_Broyden
  ImplicitNorton_Broyden2
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
  configure_file(castem${test_arg}.mtest.in
    castem${test_arg}.mtest @ONLY)
  add_test(castem${test_arg}_mtest
    ${PROJECT_BINARY_DIR}/mfront/mtest/mtest --verbose=level0 castem${test_arg}.mtest)
  set_tests_properties(castem${test_arg}_mtest
    PROPERTIES DEPENDS MFrontCastemBehaviours)
  set_tests_properties(castem${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
endmacro(test_umat)

test_umat(elasticity)
test_umat(elasticity2)
if(HAVE_CASTEM)
  test_umat(elasticity3)
  set_tests_properties(castemelasticity3_mtest
    PROPERTIES DEPENDS MFrontMaterialProperties-castem)
endif(HAVE_CASTEM)
test_umat(elasticity4)
test_umat(elasticity5)
test_umat(elasticity6)
test_umat(elasticity7)
test_umat(elasticity8)
test_umat(elasticity9)
test_umat(elasticity10)
test_umat(elasticity11)
test_umat(elasticity12)
test_umat(elasticity13)
test_umat(elasticity14)
test_umat(elasticity15)
test_umat(elasticity16)
test_umat(mazars)
test_umat(norton)
test_umat(norton2)
test_umat(stressrelaxation)
test_umat(norton-euler)
test_umat(norton-rk2)
test_umat(norton-rk4)
test_umat(orthotropiccreep)
test_umat(orthotropiccreep-rk42)
test_umat(orthotropiccreep-rkCastem)
test_umat(strainhardeningcreep)
test_umat(plasticity)
test_umat(chaboche)
test_umat(chaboche2)
