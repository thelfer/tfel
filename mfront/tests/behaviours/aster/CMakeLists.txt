if(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  set(tfel_tests_python_module "tfel_${TFEL_SUFFIX_FOR_PYTHON_MODULES}.tests_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  set(tfel_math_python_module "tfel_${TFEL_SUFFIX_FOR_PYTHON_MODULES}.math_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  set(tfel_material_python_module "tfel_${TFEL_SUFFIX_FOR_PYTHON_MODULES}.material_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  set(mfront_python_module "mfront_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  set(mfront_umatxx_python_module "mfront_${TFEL_SUFFIX_FOR_PYTHON_MODULES}.umatxx_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
  set(mtest_python_module "mtest_${TFEL_SUFFIX_FOR_PYTHON_MODULES}")
else(TFEL_PYTHON_MODULES_APPEND_SUFFIX)
  set(tfel_tests_python_module "tfel.tests")
  set(tfel_math_python_module "tfel.math")
  set(tfel_material_python_module "tfel.material")
  set(mfront_python_module "mfront")
  set(mfront_umatxx_python_module "mfront.umatxx")
  set(mtest_python_module "mtest")
endif(TFEL_PYTHON_MODULES_APPEND_SUFFIX)

configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles-30.txt"
    PolyCrystalsAngles-30.txt @ONLY)

set(mfront_tests_SOURCES
  DSLOptionsTest
  MaterialPropertiesBoundsCheck
  ExternalStateVariablesBoundsCheck
  TensorialExternalStateVariableTest
  InitializationFailureTest
  Tvergaard
  Elasticity
  Elasticity2
  Elasticity3
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity2
  LogarithmicStrainElasticity3
  OrthotropicElastic
  OrthotropicElastic2
  OrthotropicCreep
  OrthotropicCreep_rk42
  OrthotropicCreep_rkCastem
  ImplicitOrthotropicCreep
  Plasticity
  DruckerPrager
  DruckerPragerEcroLin
  DruckerPragerEcroLinDefault
  HosfordPerfectPlasticity
  JohnsonCook_s
  JohnsonCook_ssr
  JohnsonCook_ssrt
  RusinekKlepaczko_ssrt
  Norton NortonRK DDIF2
  Norton_Euler Norton_rk2
  Norton_rk4   Norton_rk42
  Norton_rkCastem
  ImplicitNorton
  ImplicitNorton2
  ImplicitNorton_Broyden
  ImplicitNorton_Broyden2
  ImplicitNorton_LevenbergMarquardt
  ImplicitNorton_NumericallyComputedJacobianBlocks
  LogarithmicStrainNortonCreep
  EllipticCreep
  StrainHardeningCreep
  Mazars
  UnilateralMazars
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
  BerveillerZaouiPolyCrystal
  BerveillerZaouiPolyCrystal_DD_CC
  BerveillerZaouiPolyCrystal_DD_CFC
  PolyCrystals
  PolyCrystals_DD_CC
  PolyCrystals_DD_CFC
  Gurson
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
  SaintVenantKirchhoffElasticityTotalLagrangian
  FiniteStrainSingleCrystal
  FiniteStrainSingleCrystal_NumericalJacobian
  ComputeStressFreeExpansion
  IsotropicLinearSwelling
  IsotropicVolumeSwelling
  OrthotropicSwelling
  OrthotropicSwelling2
  AxialGrowth
  ComputeThermalExpansion
  ComputeThermalExpansion2
  ComputeThermalExpansion3
  ParameterTest
  ParameterTest2
  GenerateMTestFileOnFailure
  ProfilerTest
  ProfilerTest2
  ProfilerTest3)
mfront_dependencies(MFrontAsterBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)

mfront_behaviour_check_library(MFrontAsterBehaviours aster
  ${mfront_tests_SOURCES})
target_link_libraries(MFrontAsterBehaviours
  PRIVATE AsterInterface)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontAsterBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

function(set_aster_test_properties test)
  foreach(rm ${IEEE754_ROUNDING_MODES})
    set(dependencies )
    list(APPEND dependencies MFrontAsterBehaviours)
    list(APPEND dependencies mtest)
    if(HAVE_CASTEM)
      list(APPEND dependencies MFrontMaterialProperties-castem)
    endif(HAVE_CASTEM)
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST ${test}
        PROPERTY DEPENDS ${dependencies}
        PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
    elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST ${test} PROPERTY DEPENDS ${dependencies})
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endfunction(set_aster_test_properties)

function(install_aster_test_file file)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/aster"
      COMPONENT mtest)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront/tests/behaviours/aster"
      COMPONENT mtest)
  endif(TFEL_APPEND_SUFFIX)
endfunction(install_aster_test_file)

function(test_aster test_arg)
  set(_XML_OUTPUT "true")
  set(_REFERENCE_FILE )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _XML_OUTPUT "false")
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      set(_REFERENCE_FILE "${_ARG}")
    endif()
  endforeach(_ARG ${ARGN})
  set(file "${CMAKE_CURRENT_SOURCE_DIR}/aster${test_arg}.mtest")
  foreach(rm ${IEEE754_ROUNDING_MODES})
    set(mtest_args )
    list(APPEND mtest_args --rounding-direction-mode=${rm})
    list(APPEND mtest_args --verbose=level0)
    list(APPEND mtest_args --xml-output=${_XML_OUTPUT})
    list(APPEND mtest_args --@library@="$<TARGET_FILE:MFrontAsterBehaviours>")
    list(APPEND mtest_args --@XMLOutputFile="${test_arg}-${rm}.xml")
    list(APPEND mtest_args ${file})
    if(HAVE_CASTEM)
      list(APPEND mtest_args --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>")
    endif(HAVE_CASTEM)
    if(_REFERENCE_FILE)
      set(rfile "${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
      list(APPEND mtest_args --@reference_file@="${rfile}")
    endif(_REFERENCE_FILE)
    add_test(NAME aster-${test_arg}_${rm}_mtest
             COMMAND mtest ${mtest_args})
    if(_WILL_FAIL)
      set_tests_properties(aster-${test_arg}_${rm}_mtest
	PROPERTIES WILL_FAIL true)
    endif(_WILL_FAIL)
    set_aster_test_properties("aster-${test_arg}_${rm}_mtest")
  endforeach(rm ${IEEE754_ROUNDING_MODES})
  install_aster_test_file("${file}")
endfunction(test_aster)

macro(test_aster_python test)
  if(NOT CMAKE_CONFIGURATION_TYPES)
    configure_file(aster${test}.py.in aster${test}.py.in2 @ONLY)
    file(GENERATE OUTPUT aster${test}.py
                  INPUT ${CMAKE_CURRENT_BINARY_DIR}/aster${test}.py.in2)
    add_test(NAME aster${test}_py
      COMMAND ${Python_EXECUTABLE} aster${test}.py)
    set_tests_properties(aster${test}_py
      PROPERTIES DEPENDS MFrontAsterBehaviours)
    set(python_env)
    list(APPEND python_env "PYTHONPATH=${PROJECT_BINARY_DIR}/bindings/python")
    list(APPEND python_env "MFrontAsterBehavioursBuildPath=$<TARGET_FILE:MFrontAsterBehaviours>")
    set_tests_properties(aster${test}_py
      PROPERTIES ENVIRONMENT "${python_env}")
  endif(NOT CMAKE_CONFIGURATION_TYPES)
endmacro(test_aster_python)

test_aster(initializationfailuretest
  NO_XML_OUTPUT WILL_FAIL)
test_aster(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_aster(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_aster(materialpropertiesboundscheck3)
test_aster(materialpropertiesboundscheck4)
test_aster(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_aster(materialpropertiesboundscheck6)

test_aster(externalstatevariablesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_aster(externalstatevariablesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_aster(externalstatevariablesboundscheck3)
test_aster(externalstatevariablesboundscheck4)
test_aster(externalstatevariablesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)

test_aster(elasticity)
test_aster(elasticity2)
if(HAVE_CASTEM)
  test_aster(elasticity3)
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
test_aster(elasticity15 REFERENCE_FILE elasticity15-aster.ref)
test_aster(elasticity16)
test_aster(elasticity18)
test_aster(elasticity19)
test_aster(elasticity20)
test_aster(elasticity21)
test_aster(elasticity22)
test_aster(elasticity23)
test_aster(elasticity33)
test_aster(druckerprager REFERENCE_FILE druckerprager.ref)
test_aster(druckerprager2 REFERENCE_FILE druckerprager2.ref)
test_aster(mazars REFERENCE_FILE mazars-aster.ref)
test_aster(mazars2 REFERENCE_FILE mazars-aster.ref)
test_aster(unilateralmazars REFERENCE_FILE unilateralmazars.ref)
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
test_aster(implicitnorton_numericallycomputedjacobianblocks)
test_aster(plasticity REFERENCE_FILE plasticity.ref)
test_aster(ellipticcreep)
test_aster(strainhardeningcreep)
test_aster(plasisocin REFERENCE_FILE plasisocin.ref)
test_aster(viscoplasisocin REFERENCE_FILE viscoplasisocin.ref)
test_aster(chaboche REFERENCE_FILE chaboche-aster.ref)
test_aster(chaboche2 REFERENCE_FILE chaboche2-aster.ref)
test_aster(viscochaboche REFERENCE_FILE ViscoChaboche.ref)
test_aster(monocrystal REFERENCE_FILE monocrystal.ref)
test_aster(monocrystal-2)
test_aster(implicitmonocrystal)
test_aster(implicitmonocrystal-2)
test_aster(monocrystal_dd_cfc
           REFERENCE_FILE monocrystal_dd_cfc.ref)
test_aster(monocrystal_dd_cfc_numericaljacobian
           REFERENCE_FILE monocrystal_dd_cfc.ref)
test_aster(monocrystal_dd_cfc_irradiation
           REFERENCE_FILE monocrystal_dd_cfc_irradiation.ref)
test_aster(monocrystal_dd_cc
           REFERENCE_FILE monocrystal_dd_cc_numericaljacobian.ref)
test_aster(monocrystal_dd_cc_numericaljacobian
           REFERENCE_FILE monocrystal_dd_cc_numericaljacobian.ref)
test_aster(monocrystal_dd_cc_irradiation
           REFERENCE_FILE monocrystal_dd_cc_irradiation_numericaljacobian.ref)
test_aster(monocrystal_dd_cc_irradiation_numericaljacobian
           REFERENCE_FILE monocrystal_dd_cc_irradiation_numericaljacobian.ref)
test_aster(polycrystals REFERENCE_FILE polycrystals.ref)
test_aster(polycrystals_dd_cc REFERENCE_FILE polycrystals_dd_cc.ref)
test_aster(polycrystals_dd_cfc  REFERENCE_FILE polycrystals_dd_cfc.ref)
test_aster(implicithayhurst)
test_aster(burger REFERENCE_FILE burger.ref)
test_aster(ageingburger REFERENCE_FILE ageingburger.ref)
test_aster(gursontvergaardneedlemanplasticflow)
test_aster(gursontvergaardneedlemanviscoplasticflow)
test_aster(anistropiclemaitreviscoplasticbehaviour REFERENCE_FILE AnistropicLemaitreViscoplasticBehaviour.ref)
test_aster(tvergaard REFERENCE_FILE tvergaard.ref)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_aster(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
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
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig11-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig22-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig33-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig11-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig22-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-sig33-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-1-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-2-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-3-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-1-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-2-2D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-1-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-2-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-uniaxialloading-3-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-1-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-2-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-3-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-4-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-5-3D)
test_aster(saintvenantkirchhoffelasticitytotallagrangian-shear-6-3D)
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
test_aster(finitestrainsinglecrystal-001
           REFERENCE_FILE finitestrainsinglecrystal-001.ref)
test_aster(finitestrainsinglecrystal-012
           REFERENCE_FILE finitestrainsinglecrystal-012.ref)
test_aster(finitestrainsinglecrystal-012-2
           REFERENCE_FILE finitestrainsinglecrystal-012.ref)
test_aster(finitestrainsinglecrystal-159
           REFERENCE_FILE finitestrainsinglecrystal-159.ref)
test_aster(finitestrainsinglecrystal_numericaljacobian-001
           REFERENCE_FILE finitestrainsinglecrystal-001.ref)
test_aster(finitestrainsinglecrystal_numericaljacobian-012
           REFERENCE_FILE finitestrainsinglecrystal-012.ref)
test_aster(finitestrainsinglecrystal_numericaljacobian-012-2
           REFERENCE_FILE finitestrainsinglecrystal-012.ref)
test_aster(finitestrainsinglecrystal_numericaljacobian-159
           REFERENCE_FILE finitestrainsinglecrystal-159.ref)
test_aster(computestressfreeexpansion)
test_aster(isotropiclinearswelling)
test_aster(isotropicvolumeswelling)
test_aster(orthotropicswelling)
test_aster(orthotropicswelling2)
test_aster(orthotropicswelling3)
test_aster(axialgrowth)
test_aster(axialgrowth2)
test_aster(rusinekklepaczko_ssrt REFERENCE_FILE rusinek-klepaczko_ssrt.ref)
test_aster(johnsoncook_ssrt REFERENCE_FILE johnson-cook_ssrt.ref)
test_aster(parametertest)
test_aster(parametertest2)
test_aster(tensorialexternalstatevariabletest)

test_aster(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-1-3D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-2-3D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-3-3D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-shear-2D
           REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-shear-FXY-3D
           REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-shear-FXZ-3D
           REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-shear-FYX-3D
           REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-shear-FYZ-3D
           REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-shear-FZX-3D
           REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-shear-FZY-3D
           REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D)
test_aster(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_aster(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D
           REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_aster(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D
           REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_aster(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D
           REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)

if(enable-python-bindings)
  test_aster_python(xx)
  test_aster_python(viscochaboche-memo-nrad)
endif(enable-python-bindings)
