add_subdirectory(dgibi)

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

configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCristalsAngles.txt"
    PolyCristalsAngles.txt @ONLY)

set(mfront_tests_SOURCES
  Elasticity Plasticity
  DruckerPrager
  Norton DDIF2
  Norton_Euler Norton_rk2
  Norton_rk4   Norton_rk42
  Norton_rkCastem
  NortonRK
  NortonRK2
  NortonRK3
  NortonRK4
  ImplicitNorton
  ImplicitFiniteStrainNorton
  ImplicitNortonPlaneStress
  ImplicitNortonGeneralisedPlaneStress
  ImplicitNorton2
  ImplicitNorton_Broyden
  ImplicitNorton_Broyden2
  EllipticCreep
  OrthotropicCreep
  OrthotropicCreep_rk42
  OrthotropicCreep_rkCastem
  ImplicitOrthotropicCreep
  OrthotropicElastic
  StrainHardeningCreep
  Lorentz
  Mazars
  Chaboche
  PlasIsoCin
  ViscoPlasIsoCin
  Chaboche2
  ImplicitMonoCristal
  MonoCristal
  MonoCristal_DD_CFC
  PolyCristals
  PolyCristals_DD_CFC
  Hayhurst
  ImplicitHayhurst
  Burger
  Tvergaard
  SaintVenantKirchhoffElasticity
  CompressibleNeoHookeanBehaviour
  IncompressibleMooneyRivlinBehaviour
  IncompressiblePlaneStressMooneyRivlinBehaviour
  IncompressiblePlaneStressGornetDesmoratBehaviour)
  
mfront_behaviour_check_library(MFrontCastemBehaviours umat  ${mfront_tests_SOURCES})
target_link_libraries(MFrontCastemBehaviours
  UMATInterface
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)
if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontCastemBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

macro(test_castem test_arg)
  configure_file(castem${test_arg}.mtest.in
    castem${test_arg}.mtest @ONLY)
  add_test(NAME castem${test_arg}_mtest
    COMMAND mtest --verbose=level0 castem${test_arg}.mtest)
  set_tests_properties(castem${test_arg}_mtest
    PROPERTIES DEPENDS MFrontCastemBehaviours)
  set_tests_properties(castem${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
  add_custom_target(castem${test_arg}.mtest-install
    ${CMAKE_COMMAND} -E copy castem${test_arg}.mtest
    ${CMAKE_INSTALL_PREFIX}/share/mfront/tests/behaviours/castem/castem${test_arg}.mtest)
  add_dependencies(tests-install
    castem${test_arg}.mtest-install)
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
test_castem(lorentz)
test_castem(mazars)
test_castem(druckerprager)
test_castem(druckerprager2)
test_castem(norton)
test_castem(norton2)
test_castem(nortonrk2)
test_castem(nortonrk3)
test_castem(nortonrk4)
test_castem(implicitnorton-planestress)
test_castem(implicitnorton-planestress2)
test_castem(implicitnorton-generalisedplanestress)
test_castem(stressrelaxation)
test_castem(norton-euler)
test_castem(norton-rk2)
test_castem(norton-rk4)
test_castem(orthotropiccreep)
test_castem(orthotropiccreep-rk42)
test_castem(orthotropiccreep-rkCastem)
test_castem(strainhardeningcreep)
test_castem(plasticity)
test_castem(plasisocin)
test_castem(viscoplasisocin)
test_castem(chaboche)
test_castem(chaboche2)
test_castem(monocristal)
test_castem(monocristal_dd_cfc)
test_castem(polycristals)
test_castem(polycristals_dd_cfc)
test_castem(hayhurst)
test_castem(implicithayhurst)
test_castem(ellipticcreep)
test_castem(burger)
test_castem(tvergaard)
