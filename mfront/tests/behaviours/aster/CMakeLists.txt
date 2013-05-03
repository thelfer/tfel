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
  ImplicitNorton_Broyden
  ImplicitNorton_Broyden2
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
  configure_file(aster${test_arg}.mtest.in
    aster${test_arg}.mtest @ONLY)
  add_test(aster${test_arg}_mtest 
    ${PROJECT_BINARY_DIR}/mfront/mtest/mtest --verbose=level0 aster${test_arg}.mtest)
  set_tests_properties(aster${test_arg}_mtest
    PROPERTIES DEPENDS MFrontAsterBehaviours)
  set_tests_properties(aster${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
endmacro(test_aster)

test_aster(elasticity)
test_aster(elasticity2)
if(HAVE_CASTEM)
  test_aster(elasticity3)
  set_tests_properties(asterelasticity3_mtest
    PROPERTIES DEPENDS MFrontMaterialProperties-castem)
endif(HAVE_CASTEM)
test_aster(elasticity4)
test_aster(elasticity5)
test_aster(elasticity6)
test_aster(elasticity7)
test_aster(elasticity8)
test_aster(elasticity9)
test_aster(elasticity10)
test_aster(elasticity11)
test_aster(elasticity12)
test_aster(elasticity13)
test_aster(elasticity14)
test_aster(elasticity15)
test_aster(elasticity16)
test_aster(mazars)
test_aster(norton)
test_aster(norton2)
# test_aster(orthotropiccreep)
# test_aster(orthotropiccreep-rk42)
# test_aster(orthotropiccreep-rkCastem)
test_aster(implicitnorton)
test_aster(implicitnorton2)
test_aster(implicitnorton_broyden)
test_aster(implicitnorton_broyden2)
test_aster(implicitorthotropiccreep)
test_aster(plasticity)
test_aster(strainhardeningcreep)
test_aster(chaboche)
test_aster(chaboche2)
