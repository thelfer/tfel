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
  NortonRK
  NortonRK2
  NortonRK3
  NortonRK4
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
  Chaboche2
  MonoCristal
  Hayhurst
  Burger)
  
mfront_behaviour_check_library(MFrontCastemBehaviours umat  ${mfront_tests_SOURCES})
target_link_libraries(MFrontCastemBehaviours
  UMATInterface
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)

macro(test_castem test_arg)
  configure_file(castem${test_arg}.mtest.in
    castem${test_arg}.mtest @ONLY)
  add_test(castem${test_arg}_mtest
    ${PROJECT_BINARY_DIR}/mfront/mtest/mtest --verbose=level0 castem${test_arg}.mtest)
  set_tests_properties(castem${test_arg}_mtest
    PROPERTIES DEPENDS MFrontCastemBehaviours)
  set_tests_properties(castem${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
endmacro(test_castem)

test_castem(elasticity)
test_castem(elasticity2)
if(HAVE_CASTEM)
  test_castem(elasticity3)
  set_tests_properties(castemelasticity3_mtest
    PROPERTIES DEPENDS MFrontMaterialProperties-castem)
endif(HAVE_CASTEM)
test_castem(elasticity4)
test_castem(elasticity5)
test_castem(elasticity6)
test_castem(elasticity7)
test_castem(elasticity8)
test_castem(elasticity9)
test_castem(elasticity10)
test_castem(elasticity11)
test_castem(elasticity12)
test_castem(elasticity13)
test_castem(elasticity14)
test_castem(elasticity15)
test_castem(elasticity16)
test_castem(mazars)
test_castem(norton)
test_castem(norton2)
test_castem(nortonrk2)
test_castem(nortonrk3)
test_castem(nortonrk4)
test_castem(stressrelaxation)
test_castem(norton-euler)
test_castem(norton-rk2)
test_castem(norton-rk4)
test_castem(orthotropiccreep)
test_castem(orthotropiccreep-rk42)
test_castem(orthotropiccreep-rkCastem)
test_castem(strainhardeningcreep)
test_castem(plasticity)
test_castem(chaboche)
test_castem(chaboche2)
test_castem(monocristal)
test_castem(hayhurst)
test_castem(burger)
