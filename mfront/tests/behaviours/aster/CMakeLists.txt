include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
                 "${PROJECT_BINARY_DIR}/src/Material"
                 "${PROJECT_BINARY_DIR}/src/Math"
                 "${PROJECT_BINARY_DIR}/src/Utilities"
                 "${PROJECT_BINARY_DIR}/src/Exception")

configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles-30.txt"
    PolyCrystalsAngles-30.txt @ONLY)

set(mfront_tests_SOURCES
  MaterialPropertiesBoundsCheck
  ExternalStateVariablesBoundsCheck
  Tvergaard
  Elasticity
  OrthotropicElastic
  OrthotropicCreep
  OrthotropicCreep_rk42
  OrthotropicCreep_rkCastem
  ImplicitOrthotropicCreep
  Plasticity
  DruckerPrager
  DruckerPragerEcroLin
  DruckerPragerEcroLinDefault
  Norton NortonRK DDIF2
  Norton_Euler Norton_rk2
  Norton_rk4   Norton_rk42
  Norton_rkCastem
  ImplicitNorton
  ImplicitNorton2
  ImplicitNorton_Broyden
  ImplicitNorton_Broyden2
  ImplicitNorton_LevenbergMarquardt
  EllipticCreep
  StrainHardeningCreep
  Mazars
  Lorentz
  Lorentz2
  PlasIsoCin
  ViscoPlasIsoCin
  Chaboche
  Chaboche2
  ViscoChaboche
  ViscoMemoNrad
  ImplicitMonoCrystal
  MonoCrystal
  MonoCrystal_DD_CFC
  MonoCrystal_DD_CFC_NumericalJacobian
  MonoCrystal_DD_CFC_Irradiation
  MonoCrystal_DD_CC
  MonoCrystal_DD_CC_LevenbergMarquardt
  MonoCrystal_DD_CC_NumericalJacobian
  MonoCrystal_DD_CC_Irradiation
  MonoCrystal_DD_CC_Irradiation_NumericalJacobian
  PolyCrystals
  PolyCrystals_DD_CC
  PolyCrystals_DD_CFC
  GursonTvergaardNeedlemanPlasticFlow_NumericalJacobian
  GursonTvergaardNeedlemanViscoPlasticFlow_NumericalJacobian
  Hayhurst
  ImplicitHayhurst
  Burger
  AgeingBurger
  AnistropicLemaitreViscoplasticBehaviour
  SaintVenantKirchhoffElasticity
  SaintVenantKirchhoffElasticity2
  SaintVenantKirchhoffElasticity3
  FiniteStrainSingleCrystal
  FiniteStrainSingleCrystal_NumericalJacobian)

if(NOT WIN32)
  set(mfront_tests_SOURCES
      Elasticity2
      Elasticity3
      OrthotropicElastic2
      ${mfront_tests_SOURCES})

mfront_dependencies(MFrontAsterBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)
endif(NOT WIN32)

mfront_behaviour_check_library(MFrontAsterBehaviours  aster ${mfront_tests_SOURCES})
target_link_libraries(MFrontAsterBehaviours
  AsterInterface
  TFELMaterial
  TFELMath
  TFELUtilities
  TFELException)
if(enable-cxx-11)
  target_link_libraries(MFrontAsterBehaviours
    MFrontProfiling)
endif(enable-cxx-11)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontAsterBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

get_property(MFrontAsterBehavioursBuildPath TARGET MFrontAsterBehaviours PROPERTY LOCATION)
if(HAVE_CASTEM)
  get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)
endif(HAVE_CASTEM)

macro(test_aster test_arg)
  configure_file(aster${test_arg}.mtest.in
    aster${test_arg}.mtest @ONLY)
  add_test(NAME aster${test_arg}_mtest
    COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false aster${test_arg}.mtest)
  set_tests_properties(aster${test_arg}_mtest
    PROPERTIES DEPENDS MFrontAsterBehaviours)
  set_tests_properties(aster${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
  set_tests_properties(aster${test_arg}_mtest
    PROPERTIES DEPENDS mtest)
  add_custom_target(aster${test_arg}.mtest-install
    ${CMAKE_COMMAND} -E copy aster${test_arg}.mtest
    ${CMAKE_INSTALL_PREFIX}/share/mfront/tests/behaviours/aster/aster${test_arg}.mtest)
  add_dependencies(tests-install
    aster${test_arg}.mtest-install)
endmacro(test_aster)

macro(test_aster_python test)
  configure_file(aster${test}.py.in
    aster${test}.py @ONLY)
  add_test(NAME aster${test}_py
    COMMAND ${PYTHON_EXECUTABLE} aster${test}.py)
  set_tests_properties(aster${test}_py
    PROPERTIES DEPENDS MFrontAsterBehaviours)
  set_tests_properties(aster${test}_py
    PROPERTIES ENVIRONMENT "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python")
endmacro(test_aster_python)

test_aster(materialpropertiesboundscheck)
test_aster(materialpropertiesboundscheck2)
test_aster(materialpropertiesboundscheck3)
test_aster(materialpropertiesboundscheck4)
test_aster(materialpropertiesboundscheck5)
test_aster(materialpropertiesboundscheck6)
set_tests_properties(astermaterialpropertiesboundscheck_mtest
  astermaterialpropertiesboundscheck2_mtest
  astermaterialpropertiesboundscheck5_mtest
  PROPERTIES WILL_FAIL true)

test_aster(externalstatevariablesboundscheck)
test_aster(externalstatevariablesboundscheck2)
test_aster(externalstatevariablesboundscheck3)
test_aster(externalstatevariablesboundscheck4)
test_aster(externalstatevariablesboundscheck5)
set_tests_properties(asterexternalstatevariablesboundscheck_mtest
  asterexternalstatevariablesboundscheck2_mtest
  asterexternalstatevariablesboundscheck5_mtest
  PROPERTIES WILL_FAIL true)

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
test_aster(mazars2)
test_aster(lorentz)
test_aster(lorentz2)
test_aster(norton)
test_aster(norton2)
test_aster(stressrelaxation)
# test_aster(norton-euler)
# test_aster(norton-rk2)
# test_aster(norton-rk4)
test_aster(orthotropiccreep)
test_aster(orthotropiccreep-rk42)
test_aster(orthotropiccreep-rkCastem)
test_aster(implicitnorton)
test_aster(implicitnorton2)
test_aster(implicitnorton3)
test_aster(implicitnorton_broyden)
test_aster(implicitnorton_broyden2)
test_aster(implicitorthotropiccreep)
test_aster(implicitnorton_levenbergmarquardt)
test_aster(plasticity)
test_aster(ellipticcreep)
test_aster(strainhardeningcreep)
test_aster(plasisocin)
test_aster(viscoplasisocin)
test_aster(chaboche)
test_aster(chaboche2)
test_aster(viscochaboche)
test_aster(monocrystal)
test_aster(monocrystal-2)
test_aster(implicitmonocrystal)
test_aster(implicitmonocrystal-2)
test_aster(monocrystal_dd_cfc)
test_aster(monocrystal_dd_cfc_numericaljacobian)
test_aster(monocrystal_dd_cfc_irradiation)
test_aster(monocrystal_dd_cc)
test_aster(monocrystal_dd_cc_numericaljacobian)
test_aster(monocrystal_dd_cc_irradiation)
test_aster(monocrystal_dd_cc_irradiation_numericaljacobian)
test_aster(polycrystals)
test_aster(polycrystals_dd_cc)
test_aster(polycrystals_dd_cfc)
test_aster(implicithayhurst)
test_aster(burger)
test_aster(ageingburger)
test_aster(gursontvergaardneedlemanplasticflow)
test_aster(gursontvergaardneedlemanviscoplasticflow)
test_aster(anistropiclemaitreviscoplasticbehaviour)
test_aster(tvergaard)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
test_aster(saintvenantkirchhoffelasticity-shear-1-2D)
test_aster(saintvenantkirchhoffelasticity-shear-2-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_aster(saintvenantkirchhoffelasticity-shear-1-3D)
test_aster(saintvenantkirchhoffelasticity-shear-2-3D)
test_aster(saintvenantkirchhoffelasticity-shear-3-3D)
test_aster(saintvenantkirchhoffelasticity-shear-4-3D)
test_aster(saintvenantkirchhoffelasticity-shear-5-3D)
test_aster(saintvenantkirchhoffelasticity-shear-6-3D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-1-2D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-2-2D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-3-2D)
test_aster(saintvenantkirchhoffelasticity2-shear-1-2D)
test_aster(saintvenantkirchhoffelasticity2-shear-2-2D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-1-3D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-2-3D)
test_aster(saintvenantkirchhoffelasticity2-uniaxialloading-3-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-1-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-2-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-3-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-4-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-5-3D)
test_aster(saintvenantkirchhoffelasticity2-shear-6-3D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-1-2D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-2-2D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-3-2D)
test_aster(saintvenantkirchhoffelasticity3-shear-1-2D)
test_aster(saintvenantkirchhoffelasticity3-shear-2-2D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-1-3D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-2-3D)
test_aster(saintvenantkirchhoffelasticity3-uniaxialloading-3-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-1-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-2-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-3-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-4-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-5-3D)
test_aster(saintvenantkirchhoffelasticity3-shear-6-3D)
test_aster(finitestrainsinglecrystal-001)
test_aster(finitestrainsinglecrystal-012)
test_aster(finitestrainsinglecrystal-012-2)
test_aster(finitestrainsinglecrystal-159)
test_aster(finitestrainsinglecrystal_numericaljacobian-001)
test_aster(finitestrainsinglecrystal_numericaljacobian-012)
test_aster(finitestrainsinglecrystal_numericaljacobian-012-2)
test_aster(finitestrainsinglecrystal_numericaljacobian-159)

if(NOT WIN32)
  test_aster(elasticity18)
  test_aster(elasticity19)
  test_aster(elasticity20)
  test_aster(elasticity21)
  test_aster(elasticity22)
  test_aster(elasticity23)
  test_aster(elasticity24)
  test_aster(elasticity25)
  test_aster(elasticity26)
  test_aster(elasticity27)
endif(NOT WIN32)

if(enable-python-bindings)
  test_aster_python(xx)
  test_aster_python(viscochaboche-memo-nrad)
endif(enable-python-bindings)
