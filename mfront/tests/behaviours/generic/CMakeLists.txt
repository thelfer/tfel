set(MFRONT_GENERIC_INTERFACES_STRESS_MEASURES )
list(APPEND MFRONT_GENERIC_INTERFACES_STRESS_MEASURES "CAUCHY")
list(APPEND MFRONT_GENERIC_INTERFACES_STRESS_MEASURES "PK1")
list(APPEND MFRONT_GENERIC_INTERFACES_STRESS_MEASURES "PK2")

set(MFRONT_GENERIC_INTERFACES_TANGENT_OPERATORS )
list(APPEND MFRONT_GENERIC_INTERFACES_TANGENT_OPERATORS "DPK1_DF")
list(APPEND MFRONT_GENERIC_INTERFACES_TANGENT_OPERATORS "DSIG_DF")
list(APPEND MFRONT_GENERIC_INTERFACES_TANGENT_OPERATORS "DS_DEGL")
list(APPEND MFRONT_GENERIC_INTERFACES_TANGENT_OPERATORS "DTAU_DDF")

configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles-30.txt"
    PolyCrystalsAngles-30.txt @ONLY)

set(mfront_tests_SOURCES
  DigitSeparatorTest
  MaterialPropertiesBoundsCheck
  StateVariablesBoundsCheck
  ExternalStateVariablesBoundsCheck
  InitializationFailureTest
  Elasticity3
  Elasticity4
  Elasticity5
  Elasticity6
  Elasticity8
  Elasticity9
  Elasticity10
  OrthotropicElastic
  OrthotropicElastic2
  OrthotropicElastic3
  OrthotropicElastic4
  OrthotropicElastic5
  OrthotropicElastic6
  OrthotropicElastic7
  OrthotropicElastic8
  OrthotropicElastic9
  PerfectPlasticity
  HosfordPerfectPlasticity
  HosfordPerfectPlasticity2
  HosfordPerfectPlasticity3
  FiniteRotationSmallStrainElasticity
  FiniteRotationSmallStrainElasticity2
  LogarithmicStrainElasticity2
  LogarithmicStrainElasticity3
  Plasticity2
  Plasticity3
  LogarithmicStrainPlasticity
  LogarithmicStrainPlasticity2
  DruckerPrager
  DruckerPragerEcroLin
  DruckerPragerEcroLinDefault
  Norton
  Norton2
  Norton5
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
  ImplicitNorton5
  ImplicitNorton6
  ThermalNorton
  ThermalNorton2
  ImplicitFiniteStrainNorton
  ImplicitNorton_Broyden
  ImplicitNorton_PowellDogLegBroyden
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
  StrainHardeningCreep4
  StrainHardeningCreep6
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
  BerveillerZaouiPolyCrystal
  BerveillerZaouiPolyCrystal_DD_CC
  BerveillerZaouiPolyCrystal_DD_CFC
  ExternalModelCaller
  ExternalModelCaller2
  ExternalModelCaller3
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
  SaintVenantKirchhoffElasticity4
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
  ParameterTest
  ParameterTest2
  ElasticMaterialPropertiesBoundsCheckTest
  TemperatureUpdateOnSubSteppingCheck
  GenerateMTestFileOnFailure
  StationaryHeatTransfer
  StationaryHeatTransfer2
  ThermoElasticity
  MiehePhaseFieldDisplacementDeviatoricSplit
  ProfilerTest
  ProfilerTest2
  ProfilerTest3
  ArrayOfScalarGradients
  ArrayOfStensorGradients
  PoroElasticity
  PoroPlasticity)

if(enable-mfront-quantity-tests)
  list(APPEND mfront_tests_SOURCES
       ComputeThermalExpansion
       ComputeThermalExpansion2
       ComputeThermalExpansion3
       ComputeThermalExpansion4
       ComputeThermalExpansion5
       ComputeThermalExpansion6
       ComputeThermalExpansion7
       Elasticity
       Elasticity2
       Elasticity7
       Norton3
       Norton4
       Plasticity
       Plasticity4
       Plasticity5
       Plasticity6
       Plasticity7
       PlasticityWithoutAutomaticTemperatureDeclaration
       StrainHardeningCreep3
       StrainHardeningCreep5
       LogarithmicStrainElasticity
       TensorialExternalStateVariableTest
       ode_default
       ode_rk54
       ode_implicit)
endif(enable-mfront-quantity-tests)

mfront_dependencies(MFrontGenericBehaviours
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
  
mfront_behaviour_check_library(MFrontGenericBehaviours generic
  ${mfront_tests_SOURCES})

if(enable-mfront-quantity-tests)
  mfront_behaviour_check_library(MFrontGenericBehaviours2 generic
    Norton_euler_qt
    Norton_rk2_qt
    Norton_rk42_qt
    Norton_rk4_qt
    Norton_rk54_qt
    Norton_rkCastem_qt
    ImplicitNorton7
    ImplicitOrthotropicCreep3
    ImplicitOrthotropicCreep4)
endif(enable-mfront-quantity-tests)

mfront_behaviour_check_library(MFrontGenericBehaviours3 generic
  DSLOptionsTest
  ElasticityParametersAsStaticVariables
  RungeKuttaElasticityParametersAsStaticVariables
  ImplicitElasticityParametersAsStaticVariables)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontGenericBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

function(set_generic_test_properties test)
  foreach(rm ${IEEE754_ROUNDING_MODES})
    set(dependencies )
    list(APPEND dependencies MFrontGenericBehaviours)
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
endfunction(set_generic_test_properties)

function(install_generic_test_file file)
  if(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/generic"
      COMPONENT mtest)
  else(TFEL_APPEND_SUFFIX)
    install(FILES ${file}
      DESTINATION "share/doc/mfront/tests/behaviours/generic"
      COMPONENT mtest)
  endif(TFEL_APPEND_SUFFIX)
endfunction(install_generic_test_file)

function(test_generic test_arg)
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
  set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
  foreach(rm ${IEEE754_ROUNDING_MODES})
    set(mtest_args )
    list(APPEND mtest_args --rounding-direction-mode=${rm})
    list(APPEND mtest_args --verbose=level0)
    list(APPEND mtest_args --xml-output=${_XML_OUTPUT})
    list(APPEND mtest_args --@library@="$<TARGET_FILE:MFrontGenericBehaviours>")
    list(APPEND mtest_args --@xml_output@="${test_arg}-${rm}.xml")
    list(APPEND mtest_args ${file})
    if(HAVE_CASTEM)
      list(APPEND mtest_args --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>")
    endif(HAVE_CASTEM)
    if(_REFERENCE_FILE)
      set(rfile "${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
      list(APPEND mtest_args --@reference_file@="${rfile}")
    endif(_REFERENCE_FILE)
    add_test(NAME generic-${test_arg}_${rm}_mtest
             COMMAND mtest ${mtest_args})
    if(_WILL_FAIL)
      set_tests_properties(generic-${test_arg}_${rm}_mtest
	PROPERTIES WILL_FAIL true)
    endif(_WILL_FAIL)
    set_generic_test_properties("generic-${test_arg}_${rm}_mtest")
  endforeach(rm ${IEEE754_ROUNDING_MODES})
  install_generic_test_file("${file}")
endfunction(test_generic)

function(test_generic_fs test_arg)
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
  set(file "${CMAKE_CURRENT_SOURCE_DIR}/${test_arg}.mtest")
  foreach(ss ${MFRONT_GENERIC_INTERFACES_STRESS_MEASURES})
  foreach(tgo ${MFRONT_GENERIC_INTERFACES_TANGENT_OPERATORS})
    foreach(rm ${IEEE754_ROUNDING_MODES})
      if(NOT _REFERENCE_FILE)
	if(HAVE_CASTEM)
	  add_test(NAME generic-${test_arg}_${ss}_${tgo}_${rm}_mtest
  	    COMMAND mtest --rounding-direction-mode=${rm}
            --verbose=level0 --xml-output=${_XML_OUTPUT}
            --result-file-output=false
            --@library@="$<TARGET_FILE:MFrontGenericBehaviours>"
	    --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>"
	    --@stress_measure@=${ss}
	    --@tangent_operator@=${tgo}
            --@xml_output@="${test_arg}-${rm}.xml" ${file})
	else(HAVE_CASTEM)
	  add_test(NAME generic-${test_arg}_${ss}_${tgo}_${rm}_mtest
  	    COMMAND mtest --rounding-direction-mode=${rm}
            --verbose=level0 --xml-output=${_XML_OUTPUT}
            --result-file-output=false
            --@library@="$<TARGET_FILE:MFrontGenericBehaviours>"
	    --@stress_measure@=${ss}
	    --@tangent_operator@=${tgo}
            --@xml_output@="${test_arg}-${rm}.xml" ${file})
	endif(HAVE_CASTEM)
      else(NOT _REFERENCE_FILE)
	set(rfile "${top_srcdir}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
	if(HAVE_CASTEM)
	  add_test(NAME generic-${test_arg}_${ss}_${tgo}_${rm}_mtest
            COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0
	    --xml-output=${_XML_OUTPUT} --result-file-output=false
            --@library@="$<TARGET_FILE:MFrontGenericBehaviours>"
	    --@mplibrary@="$<TARGET_FILE:MFrontMaterialProperties-castem>"
	    --@stress_measure@=${ss}
	    --@tangent_operator@=${tgo}
            --@xml_output@="${test_arg}-${rm}.xml"
            --@reference_file@="${rfile}" ${file})
	else(HAVE_CASTEM)
	  add_test(NAME generic-${test_arg}_${ss}_${tgo}_${rm}_mtest
            COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0
	    --xml-output=${_XML_OUTPUT} --result-file-output=false
            --@library@="$<TARGET_FILE:MFrontGenericBehaviours>"
	    --@stress_measure@=${ss}
	    --@tangent_operator@=${tgo}
            --@xml_output@="${test_arg}-${rm}.xml"
            --@reference_file@="${rfile}" ${file})
	endif(HAVE_CASTEM)
      endif(NOT _REFERENCE_FILE)
      if(_WILL_FAIL)
	set_tests_properties(generic-${test_arg}_${ss}_${tgo}_${rm}_mtest
	  PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      set_generic_test_properties("generic-${test_arg}_${ss}_${tgo}_${rm}_mtest")
    endforeach(rm ${IEEE754_ROUNDING_MODES})
  endforeach(tgo ${MFRONT_GENERIC_INTERFACES_TANGENT_OPERATORS})
  endforeach(ss ${MFRONT_GENERIC_INTERFACES_STRESS_MEASURES})
  install_generic_test_file("${file}")
endfunction(test_generic_fs)

test_generic(InitializationFailureTest
  NO_XML_OUTPUT WILL_FAIL)
test_generic(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_generic(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_generic(materialpropertiesboundscheck3)
test_generic(materialpropertiesboundscheck4)
test_generic(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
  
test_generic(statevariablesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_generic(statevariablesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)

test_generic(externalstatevariablesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_generic(externalstatevariablesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_generic(externalstatevariablesboundscheck3)
test_generic(externalstatevariablesboundscheck4)
test_generic(externalstatevariablesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_generic(externalstatevariablesboundscheck6 
  NO_XML_OUTPUT WILL_FAIL)

test_generic(elasticity4-1)
test_generic(elasticity4-2)
test_generic(elasticity4-3)
test_generic(elasticity4-4)
test_generic(elasticity5)
test_generic(elasticity5-1)
test_generic(elasticity5-2)
test_generic(elasticity6)
test_generic(elasticity6-1)
test_generic(elasticity6-2)
test_generic(elasticity8)
test_generic(elasticity9)
test_generic(elasticity10)
test_generic(elasticity10-1)
test_generic(elasticity11)
test_generic(elasticity12)
test_generic(elasticity13)
test_generic(elasticity14)
test_generic(elasticity17)
test_generic(elasticity20)
test_generic(elasticity21)
test_generic(elasticity22)
test_generic(elasticity23)
test_generic(elasticity24)
test_generic(elasticity25)
test_generic(elasticity26)
test_generic(elasticity27)
test_generic(elasticity28)
test_generic(elasticity29)
test_generic(elasticity30)
test_generic(perfectplasticity)
test_generic(plasticity2)
test_generic(plasticity3)
# test_generic(ddif2)
# test_generic(ddif2-2)
# test_generic(ddif2-3)
# test_generic(ddif2-4)
test_generic(lorentz)
test_generic(lorentz2)
# test_generic(mazars)
# test_generic(mazars2)
# test_generic(druckerprager)
# test_generic(druckerprager2)
# test_generic(norton)
# test_generic(norton2)
# test_generic(nortonrk2)
# test_generic(nortonrk3)
# test_generic(nortonrk4)
test_generic(implicitnorton)
test_generic(implicitnorton2)
test_generic(implicitnorton-planestress)
test_generic(implicitnorton5)
test_generic(implicitnorton6)
test_generic(implicitnorton-smallstraintridimensionbehaviourwrapper)
# test_generic(implicitnorton-levenbergmarquardt)
# test_generic(implicitnorton4-planestress)
# test_generic(implicitorthotropiccreep2-planestress)
# test_generic(stressrelaxation)
# test_generic(norton-euler)
# test_generic(norton-rk2)
# test_generic(norton-rk4)
test_generic(orthotropicelasticity)
test_generic(orthotropicelasticity-2)
test_generic(orthotropicelasticity3-1)
test_generic(orthotropicelasticity3-2)
test_generic(orthotropicelasticity3-3)
test_generic(orthotropicelasticity4-1)
test_generic(orthotropicelasticity5-1)
test_generic(orthotropicelasticity5-2)
test_generic(orthotropicelasticity5-3)
test_generic(orthotropicelasticity5-4)
test_generic(orthotropicelasticity5-5)
test_generic(orthotropicelasticity5-6)
test_generic(orthotropicelasticity5-7)
test_generic(orthotropicelasticity5-8)
test_generic(orthotropicelasticity5-9)
test_generic(orthotropicelasticity5-10)
test_generic(orthotropicelasticity5-11)
test_generic(orthotropicelasticity5-12)
test_generic(orthotropicelasticity5-13)
test_generic(orthotropicelasticity5-14)
test_generic(orthotropicelasticity5-15)
test_generic(orthotropicelasticity5-16)
test_generic(orthotropicelasticity5-17)
test_generic(orthotropicelasticity5-18)
test_generic(orthotropicelasticity5-19)
test_generic(orthotropicelasticity5-20)
test_generic(orthotropicelasticity5-21)
test_generic(orthotropicelasticity5-22)
test_generic(orthotropicelasticity5-23)
test_generic(orthotropicelasticity5-24)
test_generic(orthotropicelasticity6-1)
test_generic(orthotropicelasticity6-2)
test_generic(orthotropicelasticity6-3)
test_generic(orthotropicelasticity6-4)
test_generic(orthotropicelasticity6-5)
test_generic(orthotropicelasticity6-6)
test_generic(orthotropicelasticity6-7)
test_generic(orthotropicelasticity6-8)
test_generic(orthotropicelasticity6-9)
test_generic(orthotropicelasticity6-10)
test_generic(orthotropicelasticity6-11)
test_generic(orthotropicelasticity6-12)
test_generic(orthotropicelasticity6-13)
test_generic(orthotropicelasticity6-14)
test_generic(orthotropicelasticity6-15)
test_generic(orthotropicelasticity6-16)
test_generic(orthotropicelasticity6-17)
test_generic(orthotropicelasticity6-18)
test_generic(orthotropicelasticity6-19)
test_generic(orthotropicelasticity6-20)
test_generic(orthotropicelasticity6-21)
test_generic(orthotropicelasticity6-22)
test_generic(orthotropicelasticity6-23)
test_generic(orthotropicelasticity6-24)
test_generic(orthotropicelasticity9-1)
test_generic(orthotropiccreep)
test_generic(orthotropiccreep-rk42)
test_generic(orthotropiccreep-rkCastem)
test_generic(strainhardeningcreep)
test_generic(strainhardeningcreep2)
test_generic(strainhardeningcreep3)
test_generic(strainhardeningcreep6)
# test_generic(plasticity)
# test_generic(plasisocin)
# test_generic(viscoplasisocin)
test_generic(chaboche  chaboche-aster.ref)
test_generic(chaboche2 chaboche2-aster.ref)
# test_generic(viscochaboche)
# test_generic(monocrystal)
# test_generic(monocrystal-2)
# test_generic(implicitmonocrystal)
# test_generic(implicitmonocrystal-2)
# test_generic(monocrystal_dd_cfc)
# test_generic(monocrystal_dd_cfc_numericaljacobian)
# test_generic(monocrystal_dd_cfc_irradiation)
# test_generic(monocrystal_dd_cc)
# test_generic(monocrystal_dd_cc_numericaljacobian)
# test_generic(monocrystal_dd_cc_irradiation)
# test_generic(monocrystal_dd_cc_irradiation_numericaljacobian)
# test_generic(polycrystals)
# test_generic(polycrystals_dd_cc)
# test_generic(polycrystals_dd_cfc)
# test_generic(hayhurst)
# test_generic(implicithayhurst)
# test_generic(gurson)
# test_generic(gursontvergaardneedlemanplasticflow)
# test_generic(gursontvergaardneedlemanviscoplasticflow)
# test_generic(ellipticcreep)
# test_generic(burger)
# test_generic(ageingburger)
test_generic(tvergaard tvergaard.ref)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-sig11-1D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-sig22-1D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-sig33-1D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-1-1D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-2-1D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-3-1D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
test_generic_fs(saintvenantkirchhoffelasticity-shear-1-2D)
test_generic_fs(saintvenantkirchhoffelasticity-shear-2-2D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_generic_fs(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_generic_fs(saintvenantkirchhoffelasticity-shear-1-3D)
test_generic_fs(saintvenantkirchhoffelasticity-shear-2-3D)
test_generic_fs(saintvenantkirchhoffelasticity-shear-3-3D)
test_generic_fs(saintvenantkirchhoffelasticity-shear-4-3D)
test_generic_fs(saintvenantkirchhoffelasticity-shear-5-3D)
test_generic_fs(saintvenantkirchhoffelasticity-shear-6-3D)

test_generic(saintvenantkirchhoffelasticity4-uniaxialloading-1-3D)

# test_generic(elasticity_frst-uniaxialloading-1-1D)
# test_generic(elasticity_frst-uniaxialloading-2-1D)
# test_generic(elasticity_frst-uniaxialloading-3-1D)
# test_generic(elasticity_frst-uniaxialloading-1-2D)
# test_generic(elasticity_frst-uniaxialloading-2-2D)
# test_generic(elasticity_frst-uniaxialloading-3-2D)
# test_generic(elasticity_frst-shear-1-2D)
# test_generic(elasticity_frst-shear-2-2D)
# test_generic(elasticity_frst-uniaxialloading-1-3D)
# test_generic(elasticity_frst-uniaxialloading-2-3D)
# test_generic(elasticity_frst-uniaxialloading-3-3D)
# test_generic(elasticity_frst-shear-1-3D)
# test_generic(elasticity_frst-shear-2-3D)
# test_generic(elasticity_frst-shear-3-3D)
# test_generic(elasticity_frst-shear-4-3D)
# test_generic(elasticity_frst-shear-5-3D)
# test_generic(elasticity_frst-shear-6-3D)
test_generic_fs(finiterotationsmallstrainelasticity-uniaxialloading-1-1D)
test_generic_fs(finiterotationsmallstrainelasticity-uniaxialloading-2-1D)
test_generic_fs(finiterotationsmallstrainelasticity-uniaxialloading-3-1D)
test_generic_fs(finiterotationsmallstrainelasticity-uniaxialloading-1-2D)
test_generic_fs(finiterotationsmallstrainelasticity-uniaxialloading-2-2D)
test_generic_fs(finiterotationsmallstrainelasticity-uniaxialloading-3-2D)
test_generic_fs(finiterotationsmallstrainelasticity-shear-1-2D)
test_generic_fs(finiterotationsmallstrainelasticity-shear-2-2D)
test_generic_fs(finiterotationsmallstrainelasticity-uniaxialloading-1-3D)
test_generic_fs(finiterotationsmallstrainelasticity-uniaxialloading-2-3D)
test_generic_fs(finiterotationsmallstrainelasticity-uniaxialloading-3-3D)
test_generic_fs(finiterotationsmallstrainelasticity-shear-1-3D)
test_generic_fs(finiterotationsmallstrainelasticity-shear-2-3D)
test_generic_fs(finiterotationsmallstrainelasticity-shear-3-3D)
test_generic_fs(finiterotationsmallstrainelasticity-shear-4-3D)
test_generic_fs(finiterotationsmallstrainelasticity-shear-5-3D)
test_generic_fs(finiterotationsmallstrainelasticity-shear-6-3D)
test_generic_fs(finiterotationsmallstrainelasticity2-uniaxialloading-1-1D)
test_generic_fs(finiterotationsmallstrainelasticity2-uniaxialloading-2-1D)
test_generic_fs(finiterotationsmallstrainelasticity2-uniaxialloading-3-1D)
test_generic_fs(finiterotationsmallstrainelasticity2-uniaxialloading-1-2D)
test_generic_fs(finiterotationsmallstrainelasticity2-uniaxialloading-2-2D)
test_generic_fs(finiterotationsmallstrainelasticity2-uniaxialloading-3-2D)
test_generic_fs(finiterotationsmallstrainelasticity2-shear-1-2D)
test_generic_fs(finiterotationsmallstrainelasticity2-shear-2-2D)
test_generic_fs(finiterotationsmallstrainelasticity2-uniaxialloading-1-3D)
test_generic_fs(finiterotationsmallstrainelasticity2-uniaxialloading-2-3D)
test_generic_fs(finiterotationsmallstrainelasticity2-uniaxialloading-3-3D)
test_generic_fs(finiterotationsmallstrainelasticity2-shear-1-3D)
test_generic_fs(finiterotationsmallstrainelasticity2-shear-2-3D)
test_generic_fs(finiterotationsmallstrainelasticity2-shear-3-3D)
test_generic_fs(finiterotationsmallstrainelasticity2-shear-4-3D)
test_generic_fs(finiterotationsmallstrainelasticity2-shear-5-3D)
test_generic_fs(finiterotationsmallstrainelasticity2-shear-6-3D)

test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-1D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-2D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-1D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-2D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-1D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-2D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-1-3D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-2-3D)
test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-3-3D)
test_generic_fs(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D logarithmicstrainnortoncreepuniaxialtesting.ref)
test_generic_fs(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D logarithmicstrainnortoncreepuniaxialtesting.ref)
test_generic_fs(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D logarithmicstrainnortoncreepuniaxialtesting.ref)
test_generic_fs(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D logarithmicstrainnortoncreepuniaxialtesting.ref)
test_generic_fs(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D logarithmicstrainnortoncreepuniaxialtesting.ref)
test_generic_fs(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D logarithmicstrainnortoncreepuniaxialtesting.ref)
test_generic_fs(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D logarithmicstrainnortoncreepuniaxialtesting.ref)
test_generic_fs(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D logarithmicstrainnortoncreepuniaxialtesting.ref)
test_generic_fs(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D logarithmicstrainnortoncreepuniaxialtesting.ref)

# test_generic(elasticity_log1D-uniaxialloading-1-1D)
# test_generic(elasticity_log1D-uniaxialloading-2-1D)
# test_generic(elasticity_log1D-uniaxialloading-3-1D)
# test_generic(norton_log1D-uniaxialloading-1-1D)
# test_generic(norton_log1D-uniaxialloading-2-1D)
# test_generic(norton_log1D-uniaxialloading-3-1D)
# test_generic(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-1-1D)
# test_generic(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-2-1D)
# test_generic(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-3-1D)
# test_generic(elasticity_LogarithmicStrain1DWrapper-thermalstrain)
# test_generic(elasticity_LogarithmicStrain1DWrapper-thermalstrain2)
# test_generic(norton_LogarithmicStrain1DWrapper-uniaxialloading-1-1D)
# test_generic(norton_LogarithmicStrain1DWrapper-uniaxialloading-2-1D)
# test_generic(norton_LogarithmicStrain1DWrapper-uniaxialloading-3-1D)
# test_generic(elasticity_malls-uniaxialloading-1-1D)
# test_generic(elasticity_malls-uniaxialloading-1-2D)
# test_generic(elasticity_malls-uniaxialloading-1-3D)
# test_generic(elasticity_malls-uniaxialloading-2-1D)
# test_generic(elasticity_malls-uniaxialloading-2-2D)
# test_generic(elasticity_malls-uniaxialloading-2-3D)
# test_generic(elasticity_malls-uniaxialloading-3-1D)
# test_generic(elasticity_malls-uniaxialloading-3-2D)
# test_generic(elasticity_malls-uniaxialloading-3-3D)
# test_generic(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D)
# test_generic(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D)
# test_generic(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D)
# test_generic(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D)
# test_generic(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D)
# test_generic(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D)
# test_generic(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
# test_generic(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
# test_generic(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
# test_generic(incompressiblemooneyrivlinbehaviour)
# test_generic(incompressibleplanestressgornetdesmoratbehavioursheartesting)
# test_generic(incompressibleplanestressgornetdesmoratbehaviouruniaxialtesting)
# test_generic(compressibleneohookeanbehaviour)
# test_generic(incompressibleplanestressmooneyrivlinbehaviouruniaxialtesting)
# test_generic(incompressibleplanestressgornetdesmoratbehaviourbiaxialtesting)
# test_generic(implicitsimomieheelastoplasticity)
# test_generic(T91-satohtest)
# test_generic(T91-viscositytest)
test_generic_fs(finitestrainsinglecrystal-001 finitestrainsinglecrystal-001.ref)
test_generic_fs(finitestrainsinglecrystal-012 finitestrainsinglecrystal-012.ref)
test_generic_fs(finitestrainsinglecrystal-012-2 finitestrainsinglecrystal-012.ref)
test_generic_fs(finitestrainsinglecrystal-159 finitestrainsinglecrystal-159.ref)
test_generic_fs(finitestrainsinglecrystal_numericaljacobian-001 finitestrainsinglecrystal-001.ref)
test_generic_fs(finitestrainsinglecrystal_numericaljacobian-012 finitestrainsinglecrystal-012.ref)
test_generic_fs(finitestrainsinglecrystal_numericaljacobian-012-2 finitestrainsinglecrystal-012.ref)
test_generic_fs(finitestrainsinglecrystal_numericaljacobian-159 finitestrainsinglecrystal-159.ref)
# test_generic(anistropiclemaitreviscoplasticbehaviour)
# test_generic(tssf)
test_generic(computestressfreeexpansion)
test_generic(isotropiclinearswelling)
test_generic(isotropiclinearswelling2)
test_generic(isotropicvolumeswelling)
test_generic(isotropicvolumeswelling2)
test_generic(orthotropicswelling)
test_generic(orthotropicswelling2)
test_generic(orthotropicswelling3)
test_generic(axialgrowth)
test_generic(axialgrowth2)
test_generic(axialgrowth3)
test_generic(relocationtest)
test_generic(externalmodelcaller ExternalModelCallerTest.ref)
test_generic(externalmodelcaller2 ExternalModelCallerTest.ref)
test_generic(externalmodelcaller3 ExternalModelCallerTest.ref)
# test_generic(parametertest)
# test_generic(parametertest2)
test_generic(norton5)

test_generic(PoroElasticity)
test_generic(PoroPlasticity PoroPlasticity.ref)

# test_generic(elasticmaterialpropertiesboundschecktest
#   NO_XML_OUTPUT WILL_FAIL)
# test_generic(temperatureupdateonsubsteppingcheck)

test_generic(LogarithmicStrainPlasticity2)
test_generic(ThermoElasticity)
if(enable-mfront-quantity-tests)
  test_generic(elasticity)
  test_generic(elasticity2)
  if(HAVE_CASTEM)
    test_generic(elasticity3)
  endif(HAVE_CASTEM)
  test_generic(elasticity4)
  test_generic(elasticity4-0)
  test_generic(elasticity7)
  test_generic(elasticity15 elasticity15-aster.ref)
  test_generic(elasticity16)
  test_generic(elasticity18)
  test_generic(elasticity19)
  test_generic(computethermalexpansion)
  test_generic(computethermalexpansion2)
  test_generic(computethermalexpansion3)
  test_generic(ode_default ode_default.ref)
  test_generic(ode_implicit ode_implicit.ref)
  test_generic(ode_rk54 ode_rk54.ref)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-shear-2D logarithmicstrainelasticity-shear.ref)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-shear-FXY-3D logarithmicstrainelasticity-shear.ref)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-shear-FXZ-3D logarithmicstrainelasticity-shear.ref)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-shear-FYX-3D logarithmicstrainelasticity-shear.ref)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-shear-FYZ-3D logarithmicstrainelasticity-shear.ref)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-shear-FZX-3D logarithmicstrainelasticity-shear.ref)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-shear-FZY-3D logarithmicstrainelasticity-shear.ref)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-1D)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-1D)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-2D)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-1D)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-2D)
  test_generic_fs(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
  test_generic(plasticity plasticity.ref)
  test_generic(plasticity5 plasticity.ref)
  test_generic(plasticity6)
  test_generic(plasticity7 plasticity.ref)
  test_generic(plasticity_without_temperature_declaration plasticity.ref)
  test_generic(TensorialExternalStateVariableTest)
endif(enable-mfront-quantity-tests)
