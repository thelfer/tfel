add_subdirectory(dgibi)

configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles-30.txt"
    PolyCrystalsAngles-30.txt @ONLY)

set(mfront_tests_SOURCES
  DSLOptionsTest
  MaterialPropertiesBoundsCheck
  StateVariablesBoundsCheck
  ExternalStateVariablesBoundsCheck
  TensorialExternalStateVariableTest
  InitializationFailureTest
  Elasticity
  Elasticity2
  Elasticity3
  Elasticity4
  Elasticity5
  Elasticity6
  OrthotropicElastic
  OrthotropicElastic2
  OrthotropicElastic3
  OrthotropicElastic4
  OrthotropicElastic5
  OrthotropicElastic6
  GeneralizedMaxwell
  HosfordPerfectPlasticity
  FiniteRotationSmallStrainElasticity
  FiniteRotationSmallStrainElasticity2
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity2
  LogarithmicStrainElasticity3
  Plasticity
  LogarithmicStrainPlasticity
  DruckerPrager
  DruckerPragerEcroLin
  DruckerPragerEcroLinDefault
  Norton
  Norton2
  DDIF2
  Norton_Euler Norton_rk2
  Norton_rk4   Norton_rk42
  Norton_rkCastem
  NortonRK
  NortonRK2
  NortonRK3
  NortonRK4
  ImplicitNorton
  ImplicitNorton2
  ImplicitNorton3
  ImplicitNorton4
  ImplicitFiniteStrainNorton
  ImplicitNorton_Broyden
  ImplicitNorton_Broyden2
  ImplicitNorton_LevenbergMarquardt
  JohnsonCook_s
  JohnsonCook_ssr
  JohnsonCook_ssrt
  RusinekKlepaczko_ssrt
  LogarithmicStrainNortonCreep
  EllipticCreep
  OrthotropicCreep
  OrthotropicCreep_rk42
  OrthotropicCreep_rkCastem
  ImplicitOrthotropicCreep
  ImplicitOrthotropicCreep2
  StrainHardeningCreep
  StrainHardeningCreep2
  StrainHardeningCreep3
  Lorentz
  Lorentz2
  Mazars
  PlasIsoCin
  ViscoPlasIsoCin
  Chaboche
  Chaboche2
  ViscoChaboche
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
  FiniteStrainSingleCrystal
  FiniteStrainSingleCrystal2
  FiniteStrainSingleCrystal_NumericalJacobian
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
  Tvergaard
  SaintVenantKirchhoffElasticity
  SaintVenantKirchhoffElasticity2
  SaintVenantKirchhoffElasticityWithSubStepping
  OrthotropicSaintVenantKirchhoffElasticity
  CompressibleNeoHookeanBehaviour
  IncompressibleMooneyRivlinBehaviour
  IncompressiblePlaneStressMooneyRivlinBehaviour
  IncompressiblePlaneStressGornetDesmoratBehaviour
  ImplicitSimoMieheElastoPlasticity
  T91ViscoplasticBehaviour
  AnistropicLemaitreViscoplasticBehaviour
  ComputeStressFreeExpansion
  IsotropicLinearSwelling
  IsotropicLinearSwelling2
  IsotropicVolumeSwelling
  IsotropicVolumeSwelling2
  OrthotropicSwelling
  OrthotropicSwelling2
  AxialGrowth
  AxialGrowth2
  AxialGrowth3
  RelocationTest
  ComputeThermalExpansion
  ComputeThermalExpansion2
  ComputeThermalExpansion3
  ParameterTest
  ParameterTest2
  ElasticMaterialPropertiesBoundsCheckTest
  TemperatureUpdateOnSubSteppingCheck
  GenerateMTestFileOnFailure
  ProfilerTest
  ProfilerTest2
  ProfilerTest3
  ode_default
  ode_default2
  ode_rk54
  ode_implicit
  model1)

mfront_dependencies(MFrontCastemBehaviours
  Inconel600_YoungModulus
  YoungModulusTest
  PoissonRatioTest
  YoungModulusTest_1
  YoungModulusTest_2
  YoungModulusTest_3
  PoissonRatioTest_12
  PoissonRatioTest_23
  PoissonRatioTest_13
  ShearModulusTest_12
  ShearModulusTest_23
  ShearModulusTest_13
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3
  T91AusteniticSteel_ThermalExpansion_ROUX2007
  T91AusteniticSteel_b_ROUX2007
  T91AusteniticSteel_F_ROUX2007
  T91AusteniticSteel_Kvisc_ROUX2007
  T91AusteniticSteel_Nexp_ROUX2007
  T91AusteniticSteel_Rinf_ROUX2007
  T91AusteniticSteel_ThermalExpansion_ROUX2007
  T91AusteniticSteel_YieldStress_ROUX2007
  T91AusteniticSteel_YoungModulus_ROUX2007
  T91MartensiticSteel_b_ROUX2007
  T91MartensiticSteel_C1_ROUX2007
  T91MartensiticSteel_C2_ROUX2007
  T91MartensiticSteel_F_ROUX2007
  T91MartensiticSteel_gamma1_ROUX2007
  T91MartensiticSteel_growth_ROUX2007
  T91MartensiticSteel_Kvisc_ROUX2007
  T91MartensiticSteel_Nexp_ROUX2007
  T91MartensiticSteel_Rinf_ROUX2007
  T91MartensiticSteel_ThermalExpansion_ROUX2007
  T91MartensiticSteel_YieldStress_ROUX2007
  T91MartensiticSteel_YoungModulus_ROUX2007
  YoungModulusBoundsCheckTest)
  
mfront_behaviour_check_library(MFrontCastemBehaviours umat
  ${mfront_tests_SOURCES})
target_link_libraries(MFrontCastemBehaviours
  PRIVATE CastemInterface)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontCastemBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

function(test_castem test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  set(_EXPECT_ADDITIONAL_ENVIRONMENT )
  set(_ADDITIONAL_ENVIRONMENT )
  set(_EXPECT_REFERENCE_FILE)
  set(_REFERENCE_FILE )
  foreach(_ARG ${ARGN})
    if(_EXPECT_ADDITIONAL_ENVIRONMENT)
      if(_ADDITIONAL_ENVIRONMENT)
	message(FATAL_ERROR "test_castem: environment already set")
      endif(_ADDITIONAL_ENVIRONMENT)
      set(_ADDITIONAL_ENVIRONMENT "${_ARG}")
      set(_EXPECT_ADDITIONAL_ENVIRONMENT )
    elseif(_EXPECT_REFERENCE_FILE)
      if(_REFERENCE_FILE)
	message(FATAL_ERROR "test_castem: reference file already set")
      endif(_REFERENCE_FILE)
      set(_REFERENCE_FILE "${_ARG}")
      set(_EXPECT_REFERENCE_FILE )
    else()
      if ( ${_ARG} MATCHES NO_XML_OUTPUT )
	set ( _NO_XML_OUTPUT ON)
      elseif ( ${_ARG} MATCHES WILL_FAIL)
	set ( _WILL_FAIL ON)
      elseif ( ${_ARG} MATCHES ENVIRONMENT)
	set(_EXPECT_ADDITIONAL_ENVIRONMENT ON)
      elseif ( ${_ARG} MATCHES REFERENCE_FILE)
	set(_EXPECT_REFERENCE_FILE ON)
      else()
	message(FATAL_ERROR "test_castem: unsupported option '${_ARG}'")
      endif()
    endif()
  endforeach(_ARG ${ARGN})
  if(_EXPECT_ADDITIONAL_ENVIRONMENT)
    message(FATAL_ERROR "test_castem: expected environment variable")
  endif()
  if(_EXPECT_REFERENCE_FILE)
    message(FATAL_ERROR "test_castem: expected reference file")
  endif()
  if(_REFERENCE_FILE)
    set(_REFERENCE_FILE "${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
  endif(_REFERENCE_FILE)
  foreach(rm ${IEEE754_ROUNDING_MODES})
    set(test "castem${test_arg}_${rm}_mtest")
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/castem${test_arg}.mtest")
    set(_mtest_args)
    list(APPEND _mtest_args --rounding-direction-mode=${rm} )
    list(APPEND _mtest_args --verbose=level0)
    list(APPEND _mtest_args --result-file-output=false)
    list(APPEND _mtest_args --@interface@=umat)
    list(APPEND _mtest_args --@library@="$<TARGET_FILE:MFrontCastemBehaviours>")
    list(APPEND _mtest_args --@top_srcdir@=${PROJECT_SOURCE_DIR})
    list(APPEND _mtest_args --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>")
    list(APPEND _mtest_args --@reference_file@="${_REFERENCE_FILE}")
    if(_NO_XML_OUTPUT)
      list(APPEND _mtest_args --xml-output=false)
    else(_NO_XML_OUTPUT)
      list(APPEND _mtest_args --xml-output=true)
    endif(_NO_XML_OUTPUT)
    add_test(NAME ${test} COMMAND mtest ${_mtest_args} ${file})
    if(_WILL_FAIL)
      set_tests_properties(${test} PROPERTIES WILL_FAIL true)
    endif(_WILL_FAIL)
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST ${test}        
         PROPERTY ENVIRONMENT ${_ADDITIONAL_ENVIRONMENT}
        "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
    else((CMAKE_HOST_WIN32) AND (NOT MSYS))
      if(_ADDITIONAL_ENVIRONMENT)
        set_property(TEST ${test} PROPERTY ENVIRONMENT ${_ADDITIONAL_ENVIRONMENT})
      endif(_ADDITIONAL_ENVIRONMENT)
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
        DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/castem"
        COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
        DESTINATION "share/doc/mfront/tests/behaviours/castem"
        COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endfunction(test_castem)

test_castem(initializationfailuretest
  NO_XML_OUTPUT WILL_FAIL)
test_castem(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_castem(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_castem(materialpropertiesboundscheck3)
test_castem(materialpropertiesboundscheck4)
test_castem(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_castem(materialpropertiesboundscheck6
  ENVIRONMENT "CASTEM_OUT_OF_BOUNDS_POLICY=NONE")
test_castem(materialpropertiesboundscheck7
  ENVIRONMENT "CASTEM_OUT_OF_BOUNDS_POLICY=WARNING")
test_castem(materialpropertiesboundscheck8
  NO_XML_OUTPUT WILL_FAIL ENVIRONMENT "CASTEM_OUT_OF_BOUNDS_POLICY=STRICT")

test_castem(statevariablesboundscheck  NO_XML_OUTPUT WILL_FAIL)
test_castem(statevariablesboundscheck2 NO_XML_OUTPUT WILL_FAIL)

test_castem(externalstatevariablesboundscheck  NO_XML_OUTPUT WILL_FAIL)
test_castem(externalstatevariablesboundscheck2 NO_XML_OUTPUT WILL_FAIL)
test_castem(externalstatevariablesboundscheck3)
test_castem(externalstatevariablesboundscheck4)
test_castem(externalstatevariablesboundscheck5 NO_XML_OUTPUT WILL_FAIL)
test_castem(externalstatevariablesboundscheck6 NO_XML_OUTPUT WILL_FAIL)

test_castem(elasticity)
test_castem(elasticity2)
test_castem(elasticity3)
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
test_castem(elasticity15 REFERENCE_FILE elasticity15-aster.ref)
test_castem(elasticity16)
test_castem(elasticity17)
test_castem(elasticity18)
test_castem(elasticity19)
test_castem(elasticity20)
test_castem(elasticity21)
test_castem(elasticity22)
test_castem(elasticity23)
test_castem(elasticity24)
test_castem(elasticity25)
test_castem(elasticity26)
test_castem(elasticity27)
test_castem(elasticity28)
test_castem(elasticity29)
test_castem(elasticity30)
test_castem(elasticity31)
test_castem(elasticity32)
test_castem(elasticity33)
test_castem(elasticity4-1)
test_castem(elasticity4-2)
test_castem(elasticity4-3)
test_castem(elasticity4-4)
test_castem(elasticity5-1)
test_castem(elasticity5-2)
test_castem(elasticity6-1)
test_castem(elasticity6-2)
test_castem(generalizedmaxwell REFERENCE_FILE generalizedmaxwell.ref)
test_castem(ddif2 REFERENCE_FILE ddif2-uniaxialtesting.ref)
test_castem(ddif2-2 REFERENCE_FILE ddif2-uniaxialtesting.ref)
test_castem(ddif2-3 REFERENCE_FILE ddif2-uniaxialtesting.ref)
test_castem(ddif2-4 REFERENCE_FILE ddif2-biaxialtesting.ref)
test_castem(lorentz)
test_castem(lorentz2)
test_castem(mazars REFERENCE_FILE mazars-aster.ref)
test_castem(mazars2 REFERENCE_FILE mazars-aster.ref)
test_castem(druckerprager REFERENCE_FILE druckerprager.ref)
test_castem(druckerprager2 REFERENCE_FILE druckerprager2.ref)
test_castem(norton)
test_castem(norton2)
test_castem(nortonrk2)
test_castem(nortonrk3)
test_castem(nortonrk4)
test_castem(implicitnorton)
test_castem(implicitnorton2)
test_castem(implicitnorton3)
test_castem(implicitnorton-planestress)
test_castem(implicitnorton-planestress2)
test_castem(implicitnorton-levenbergmarquardt)
test_castem(implicitnorton4-planestress)
test_castem(implicitorthotropiccreep2-planestress)
test_castem(stressrelaxation)
test_castem(norton-euler)
test_castem(norton-rk2)
test_castem(norton-rk4)
test_castem(orthotropicelasticity3-1)
test_castem(orthotropicelasticity3-2)
test_castem(orthotropicelasticity3-3)
test_castem(orthotropicelasticity4-1)
test_castem(orthotropicelasticity5-1)
test_castem(orthotropicelasticity5-2)
test_castem(orthotropicelasticity5-3)
test_castem(orthotropicelasticity5-4)
test_castem(orthotropicelasticity5-5)
test_castem(orthotropicelasticity5-6)
test_castem(orthotropicelasticity5-7)
test_castem(orthotropicelasticity5-8)
test_castem(orthotropicelasticity5-9)
test_castem(orthotropicelasticity5-10)
test_castem(orthotropicelasticity5-11)
test_castem(orthotropicelasticity5-12)
test_castem(orthotropicelasticity5-13)
test_castem(orthotropicelasticity5-14)
test_castem(orthotropicelasticity5-15)
test_castem(orthotropicelasticity5-16)
test_castem(orthotropicelasticity5-17)
test_castem(orthotropicelasticity5-18)
test_castem(orthotropicelasticity5-19)
test_castem(orthotropicelasticity5-20)
test_castem(orthotropicelasticity5-21)
test_castem(orthotropicelasticity5-22)
test_castem(orthotropicelasticity5-23)
test_castem(orthotropicelasticity5-24)
test_castem(orthotropicelasticity6-1)
test_castem(orthotropicelasticity6-2)
test_castem(orthotropicelasticity6-3)
test_castem(orthotropicelasticity6-4)
test_castem(orthotropicelasticity6-5)
test_castem(orthotropicelasticity6-6)
test_castem(orthotropicelasticity6-7)
test_castem(orthotropicelasticity6-8)
test_castem(orthotropicelasticity6-9)
test_castem(orthotropicelasticity6-10)
test_castem(orthotropicelasticity6-11)
test_castem(orthotropicelasticity6-12)
test_castem(orthotropicelasticity6-13)
test_castem(orthotropicelasticity6-14)
test_castem(orthotropicelasticity6-15)
test_castem(orthotropicelasticity6-16)
test_castem(orthotropicelasticity6-17)
test_castem(orthotropicelasticity6-18)
test_castem(orthotropicelasticity6-19)
test_castem(orthotropicelasticity6-20)
test_castem(orthotropicelasticity6-21)
test_castem(orthotropicelasticity6-22)
test_castem(orthotropicelasticity6-23)
test_castem(orthotropicelasticity6-24)
test_castem(orthotropiccreep)
test_castem(orthotropiccreep-rk42)
test_castem(orthotropiccreep-rkCastem)
test_castem(strainhardeningcreep)
test_castem(strainhardeningcreep2)
test_castem(strainhardeningcreep3)
test_castem(plasticity REFERENCE_FILE plasticity.ref)
test_castem(plasisocin REFERENCE_FILE plasisocin.ref)
test_castem(viscoplasisocin REFERENCE_FILE viscoplasisocin.ref)
test_castem(chaboche REFERENCE_FILE chaboche-aster.ref)
test_castem(chaboche2 REFERENCE_FILE chaboche2-aster.ref)
test_castem(viscochaboche REFERENCE_FILE ViscoChaboche.ref)
test_castem(monocrystal REFERENCE_FILE monocrystal.ref)
test_castem(monocrystal-2)
test_castem(implicitmonocrystal REFERENCE_FILE monocrystal.ref)
test_castem(implicitmonocrystal-2)
test_castem(monocrystal_dd_cfc
  REFERENCE_FILE monocrystal_dd_cfc.ref)
test_castem(monocrystal_dd_cfc_numericaljacobian
  REFERENCE_FILE monocrystal_dd_cfc.ref)
test_castem(monocrystal_dd_cfc_irradiation 
  REFERENCE_FILE monocrystal_dd_cfc_irradiation.ref)
test_castem(monocrystal_dd_cc
  REFERENCE_FILE monocrystal_dd_cc_numericaljacobian.ref)
test_castem(monocrystal_dd_cc_numericaljacobian
  REFERENCE_FILE monocrystal_dd_cc_numericaljacobian.ref)
test_castem(monocrystal_dd_cc_irradiation
  REFERENCE_FILE monocrystal_dd_cc_irradiation_numericaljacobian.ref)
test_castem(monocrystal_dd_cc_irradiation_numericaljacobian
  REFERENCE_FILE monocrystal_dd_cc_irradiation_numericaljacobian.ref)
test_castem(polycrystals
  REFERENCE_FILE polycrystals.ref)
test_castem(polycrystals_dd_cc
  REFERENCE_FILE polycrystals_dd_cc.ref)
test_castem(polycrystals_dd_cfc
  REFERENCE_FILE polycrystals_dd_cfc.ref)
test_castem(hayhurst REFERENCE_FILE hayhurst.ref)
test_castem(implicithayhurst)
test_castem(gurson)
test_castem(gursontvergaardneedlemanplasticflow)
test_castem(gursontvergaardneedlemanviscoplasticflow)
test_castem(ellipticcreep)
test_castem(burger REFERENCE_FILE burger.ref)
test_castem(ageingburger REFERENCE_FILE ageingburger.ref)
test_castem(tvergaard REFERENCE_FILE tvergaard.ref)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-sig11-1D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-sig22-1D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-sig33-1D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-1-1D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-2-1D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-3-1D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
test_castem(saintvenantkirchhoffelasticity-shear-1-2D)
test_castem(saintvenantkirchhoffelasticity-shear-2-2D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_castem(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_castem(saintvenantkirchhoffelasticity-shear-1-3D)
test_castem(saintvenantkirchhoffelasticity-shear-2-3D)
test_castem(saintvenantkirchhoffelasticity-shear-3-3D)
test_castem(saintvenantkirchhoffelasticity-shear-4-3D)
test_castem(saintvenantkirchhoffelasticity-shear-5-3D)
test_castem(saintvenantkirchhoffelasticity-shear-6-3D)
test_castem(saintvenantkirchhoffelasticitywithsubstepping-uniaxialloading-1-3D)
test_castem(elasticity_frst-uniaxialloading-1-1D)
test_castem(elasticity_frst-uniaxialloading-2-1D)
test_castem(elasticity_frst-uniaxialloading-3-1D)
test_castem(elasticity_frst-uniaxialloading-1-2D)
test_castem(elasticity_frst-uniaxialloading-2-2D)
test_castem(elasticity_frst-uniaxialloading-3-2D)
test_castem(elasticity_frst-shear-1-2D)
test_castem(elasticity_frst-shear-2-2D)
test_castem(elasticity_frst-uniaxialloading-1-3D)
test_castem(elasticity_frst-uniaxialloading-2-3D)
test_castem(elasticity_frst-uniaxialloading-3-3D)
test_castem(elasticity_frst-shear-1-3D)
test_castem(elasticity_frst-shear-2-3D)
test_castem(elasticity_frst-shear-3-3D)
test_castem(elasticity_frst-shear-4-3D)
test_castem(elasticity_frst-shear-5-3D)
test_castem(elasticity_frst-shear-6-3D)
test_castem(finiterotationsmallstrainelasticity-uniaxialloading-1-1D)
test_castem(finiterotationsmallstrainelasticity-uniaxialloading-2-1D)
test_castem(finiterotationsmallstrainelasticity-uniaxialloading-3-1D)
test_castem(finiterotationsmallstrainelasticity-uniaxialloading-1-2D)
test_castem(finiterotationsmallstrainelasticity-uniaxialloading-2-2D)
test_castem(finiterotationsmallstrainelasticity-uniaxialloading-3-2D)
test_castem(finiterotationsmallstrainelasticity-shear-1-2D)
test_castem(finiterotationsmallstrainelasticity-shear-2-2D)
test_castem(finiterotationsmallstrainelasticity-uniaxialloading-1-3D)
test_castem(finiterotationsmallstrainelasticity-uniaxialloading-2-3D)
test_castem(finiterotationsmallstrainelasticity-uniaxialloading-3-3D)
test_castem(finiterotationsmallstrainelasticity-shear-1-3D)
test_castem(finiterotationsmallstrainelasticity-shear-2-3D)
test_castem(finiterotationsmallstrainelasticity-shear-3-3D)
test_castem(finiterotationsmallstrainelasticity-shear-4-3D)
test_castem(finiterotationsmallstrainelasticity-shear-5-3D)
test_castem(finiterotationsmallstrainelasticity-shear-6-3D)
test_castem(finiterotationsmallstrainelasticity2-uniaxialloading-1-1D)
test_castem(finiterotationsmallstrainelasticity2-uniaxialloading-2-1D)
test_castem(finiterotationsmallstrainelasticity2-uniaxialloading-3-1D)
test_castem(finiterotationsmallstrainelasticity2-uniaxialloading-1-2D)
test_castem(finiterotationsmallstrainelasticity2-uniaxialloading-2-2D)
test_castem(finiterotationsmallstrainelasticity2-uniaxialloading-3-2D)
test_castem(finiterotationsmallstrainelasticity2-shear-1-2D)
test_castem(finiterotationsmallstrainelasticity2-shear-2-2D)
test_castem(finiterotationsmallstrainelasticity2-uniaxialloading-1-3D)
test_castem(finiterotationsmallstrainelasticity2-uniaxialloading-2-3D)
test_castem(finiterotationsmallstrainelasticity2-uniaxialloading-3-3D)
test_castem(finiterotationsmallstrainelasticity2-shear-1-3D)
test_castem(finiterotationsmallstrainelasticity2-shear-2-3D)
test_castem(finiterotationsmallstrainelasticity2-shear-3-3D)
test_castem(finiterotationsmallstrainelasticity2-shear-4-3D)
test_castem(finiterotationsmallstrainelasticity2-shear-5-3D)
test_castem(finiterotationsmallstrainelasticity2-shear-6-3D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-1D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-1D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-1D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-planestress)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-planestress)
test_castem(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-1D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-1D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-1D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-shear-2D
  REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-shear-FXY-3D
  REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-shear-FXZ-3D
  REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-shear-FYX-3D
  REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-shear-FYZ-3D
  REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-shear-FZX-3D
  REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_castem(mieheapellambrechtlogarithmicstrainelasticity-shear-FZY-3D
  REFERENCE_FILE logarithmicstrainelasticity-shear.ref)
test_castem(elasticity_log1D-uniaxialloading-1-1D)
test_castem(elasticity_log1D-uniaxialloading-2-1D)
test_castem(elasticity_log1D-uniaxialloading-3-1D)
test_castem(norton_log1D-uniaxialloading-1-1D
  REFERENCE_FILE norton_log1D-uniaxialtesting.ref)
test_castem(norton_log1D-uniaxialloading-2-1D
  REFERENCE_FILE norton_log1D-uniaxialtesting.ref)
test_castem(norton_log1D-uniaxialloading-3-1D
  REFERENCE_FILE norton_log1D-uniaxialtesting.ref)
test_castem(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-1-1D)
test_castem(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-2-1D)
test_castem(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-3-1D)
test_castem(elasticity_LogarithmicStrain1DWrapper-thermalstrain)
test_castem(elasticity_LogarithmicStrain1DWrapper-thermalstrain2)
test_castem(norton_LogarithmicStrain1DWrapper-uniaxialloading-1-1D
  REFERENCE_FILE norton_log1D-uniaxialtesting.ref)
test_castem(norton_LogarithmicStrain1DWrapper-uniaxialloading-2-1D
  REFERENCE_FILE norton_log1D-uniaxialtesting.ref)
test_castem(norton_LogarithmicStrain1DWrapper-uniaxialloading-3-1D
  REFERENCE_FILE norton_log1D-uniaxialtesting.ref)
test_castem(elasticity_malls-uniaxialloading-1-1D)
test_castem(elasticity_malls-uniaxialloading-1-2D)
test_castem(elasticity_malls-uniaxialloading-1-3D)
test_castem(elasticity_malls-uniaxialloading-2-1D)
test_castem(elasticity_malls-uniaxialloading-2-2D)
test_castem(elasticity_malls-uniaxialloading-2-3D)
test_castem(elasticity_malls-uniaxialloading-3-1D)
test_castem(elasticity_malls-uniaxialloading-3-2D)
test_castem(elasticity_malls-uniaxialloading-3-3D)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D
  REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D
  REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D
  REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D
  REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D
  REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D
  REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D
  REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D
  REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D
  REFERENCE_FILE logarithmicstrainnortoncreepuniaxialtesting.ref)
test_castem(incompressiblemooneyrivlinbehaviour)
test_castem(incompressibleplanestressgornetdesmoratbehavioursheartesting)
test_castem(incompressibleplanestressgornetdesmoratbehaviouruniaxialtesting)
test_castem(compressibleneohookeanbehaviour)
test_castem(incompressibleplanestressmooneyrivlinbehaviouruniaxialtesting)
test_castem(incompressibleplanestressgornetdesmoratbehaviourbiaxialtesting)
test_castem(implicitsimomieheelastoplasticity REFERENCE_FILE implicitsimomieheelastoplasticity.ref)
test_castem(T91-satohtest REFERENCE_FILE T91-satohtest.ref)
test_castem(T91-viscositytest)
test_castem(finitestrainsinglecrystal-001)
test_castem(finitestrainsinglecrystal-012)
test_castem(finitestrainsinglecrystal-012-2)
test_castem(finitestrainsinglecrystal-159)
test_castem(finitestrainsinglecrystal_numericaljacobian-001)
test_castem(finitestrainsinglecrystal_numericaljacobian-012)
test_castem(finitestrainsinglecrystal_numericaljacobian-012-2)
test_castem(finitestrainsinglecrystal_numericaljacobian-159)
test_castem(finitestrainsinglecrystal2-001)
test_castem(anistropiclemaitreviscoplasticbehaviour
  REFERENCE_FILE AnistropicLemaitreViscoplasticBehaviour.ref) 
test_castem(tssf REFERENCE_FILE tssf.ref)
test_castem(computestressfreeexpansion)
test_castem(isotropiclinearswelling)
test_castem(isotropiclinearswelling2)
test_castem(isotropicvolumeswelling)
test_castem(isotropicvolumeswelling2)
test_castem(orthotropicswelling)
test_castem(orthotropicswelling2)
test_castem(orthotropicswelling3)
test_castem(axialgrowth)
test_castem(axialgrowth2)
test_castem(axialgrowth3)
test_castem(axialgrowth4)
test_castem(relocationtest)
test_castem(computethermalexpansion)
test_castem(computethermalexpansion2)
test_castem(computethermalexpansion3)
test_castem(parametertest)
test_castem(parametertest2)
test_castem(tensorialexternalstatevariabletest)
test_castem(ode_default REFERENCE_FILE  ode_default.ref)
test_castem(ode_implicit REFERENCE_FILE ode_implicit.ref)
test_castem(ode_rk54 REFERENCE_FILE ode_rk54.ref)
test_castem(model1)

test_castem(elasticmaterialpropertiesboundschecktest
  NO_XML_OUTPUT WILL_FAIL)
test_castem(temperatureupdateonsubsteppingcheck)

function(test_castem_cxx test_arg)
  configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/castem/${test_arg}.cxx.in"
                 "${PROJECT_BINARY_DIR}/mfront/tests/behaviours/castem/${test_arg}.cxx" @ONLY)
  add_executable(${test_arg} EXCLUDE_FROM_ALL
      ${PROJECT_BINARY_DIR}/mfront/tests/behaviours/castem/${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  set_property(TEST ${test_arg}
               PROPERTY ENVIRONMENT MFrontCastemBehavioursBuildPath=$<TARGET_FILE:MFrontCastemBehaviours>)
  target_link_libraries(${test_arg}
      TFELGlossary TFELSystem TFELUtilities TFELTests TFELException)
  set_tests_properties(${test_arg}
      PROPERTIES DEPENDS MFrontCastemBehaviours)
  add_dependencies(check ${test_arg})
endfunction(test_castem_cxx)

test_castem_cxx(UMATExternalBehaviourDescription)
test_castem_cxx(LibraryInformationCastemTest)
