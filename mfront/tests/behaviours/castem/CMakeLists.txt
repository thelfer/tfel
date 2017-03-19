add_subdirectory(dgibi)

if(CASTEM_INCLUDE_DIR)
  include_directories(
    "${CASTEM_INCLUDE_DIR}")
endif(CASTEM_INCLUDE_DIR)

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/mfront/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/mfront/src"
  "${PROJECT_BINARY_DIR}/src/Glossary"
  "${PROJECT_BINARY_DIR}/src/Material"
  "${PROJECT_BINARY_DIR}/src/Math"
  "${PROJECT_BINARY_DIR}/src/Utilities"
  "${PROJECT_BINARY_DIR}/src/System"
  "${PROJECT_BINARY_DIR}/src/Tests"
  "${PROJECT_BINARY_DIR}/src/Exception")

configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles-30.txt"
    PolyCrystalsAngles-30.txt @ONLY)

set(mfront_tests_SOURCES
  MaterialPropertiesBoundsCheck
  StateVariablesBoundsCheck
  ExternalStateVariablesBoundsCheck
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
  FiniteRotationSmallStrainElasticity
  FiniteRotationSmallStrainElasticity2
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity2
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
  LogarithmicStrainNortonCreep
  EllipticCreep
  OrthotropicCreep
  OrthotropicCreep_rk42
  OrthotropicCreep_rkCastem
  ImplicitOrthotropicCreep
  ImplicitOrthotropicCreep2
  StrainHardeningCreep
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
  AxialGrowth2)

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
  T91MartensiticSteel_YoungModulus_ROUX2007)
  
mfront_behaviour_check_library(MFrontCastemBehaviours umat  ${mfront_tests_SOURCES})

target_link_libraries(MFrontCastemBehaviours
  CastemInterface
  MFrontProfiling
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
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_castem: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  foreach(rm ${IEEE754_ROUNDING_MODES})
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(conf ${CMAKE_CONFIGURATION_TYPES})
	set(test "castem${test_arg}_${rm}_${conf}_mtest")
	set(file "castem${test_arg}-${rm}-${conf}.mtest")
	get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
	get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
	configure_file(castem${test_arg}.mtest.in
	  ${file} @ONLY)
	if(_NO_XML_OUTPUT)
	  add_test(NAME ${test}
	    COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file}
	    CONFIGURATIONS ${conf})
	else(_NO_XML_OUTPUT)
	  add_test(NAME ${test}
	    COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file}
	    CONFIGURATIONS ${conf})
	endif(_NO_XML_OUTPUT)
	if(_WILL_FAIL)
	  set_tests_properties(${test}
	    PROPERTIES WILL_FAIL true)
	endif(_WILL_FAIL)
	if((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	    PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
	elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
	endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/mfront/tests/behaviours/castem"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
    else(CMAKE_CONFIGURATION_TYPES)
      get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      set(test "castem${test_arg}_${rm}_mtest")
      set(file "castem${test_arg}-${rm}.mtest")
      configure_file(castem${test_arg}.mtest.in
	${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file})
      else(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(${test}
	  PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      set_property(TEST ${test}
	PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	DESTINATION "share/mfront/tests/behaviours/castem"
	COMPONENT mtest)
    endif(CMAKE_CONFIGURATION_TYPES)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endmacro(test_castem)

test_castem(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_castem(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_castem(materialpropertiesboundscheck3)
test_castem(materialpropertiesboundscheck4)
test_castem(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_castem(materialpropertiesboundscheck6)
test_castem(materialpropertiesboundscheck7)
test_castem(materialpropertiesboundscheck8 
  NO_XML_OUTPUT WILL_FAIL)

foreach(rm ${IEEE754_ROUNDING_MODES})
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set_property(TEST castemmaterialpropertiesboundscheck6_${rm}_${conf}_mtest
      	 APPEND PROPERTY ENVIRONMENT "& CASTEM_OUT_OF_BOUNDS_POLICY=NONE")
      set_property(TEST castemmaterialpropertiesboundscheck7_${rm}_${conf}_mtest
      	 APPEND PROPERTY ENVIRONMENT "& CASTEM_OUT_OF_BOUNDS_POLICY=WARNING")
      set_property(TEST castemmaterialpropertiesboundscheck8_${rm}_${conf}_mtest
	 APPEND PROPERTY ENVIRONMENT "& CASTEM_OUT_OF_BOUNDS_POLICY=STRICT")
      set_property(TEST castemmaterialpropertiesboundscheck_${rm}_${conf}_mtest
	castemmaterialpropertiesboundscheck2_${rm}_${conf}_mtest
	castemmaterialpropertiesboundscheck5_${rm}_${conf}_mtest
	castemmaterialpropertiesboundscheck8_${rm}_${conf}_mtest
	PROPERTY WILL_FAIL true)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    set_property(TEST castemmaterialpropertiesboundscheck6_${rm}_mtest
      APPEND PROPERTY ENVIRONMENT "CASTEM_OUT_OF_BOUNDS_POLICY=NONE")
    set_property(TEST castemmaterialpropertiesboundscheck7_${rm}_mtest
      APPEND PROPERTY ENVIRONMENT "CASTEM_OUT_OF_BOUNDS_POLICY=WARNING")
    set_property(TEST castemmaterialpropertiesboundscheck8_${rm}_mtest
      APPEND PROPERTY ENVIRONMENT "CASTEM_OUT_OF_BOUNDS_POLICY=STRICT")
    set_property(TEST castemmaterialpropertiesboundscheck_${rm}_mtest
      castemmaterialpropertiesboundscheck2_${rm}_mtest
      castemmaterialpropertiesboundscheck5_${rm}_mtest
      castemmaterialpropertiesboundscheck8_${rm}_mtest
      PROPERTY WILL_FAIL true)
  endif(CMAKE_CONFIGURATION_TYPES)
endforeach(rm ${IEEE754_ROUNDING_MODES})
  
test_castem(statevariablesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_castem(statevariablesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)

test_castem(externalstatevariablesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_castem(externalstatevariablesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_castem(externalstatevariablesboundscheck3)
test_castem(externalstatevariablesboundscheck4)
test_castem(externalstatevariablesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_castem(externalstatevariablesboundscheck6 
  NO_XML_OUTPUT WILL_FAIL)

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
test_castem(elasticity15)
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
test_castem(generalizedmaxwell)
test_castem(ddif2)
test_castem(ddif2-2)
test_castem(ddif2-3)
test_castem(ddif2-4)
test_castem(lorentz)
test_castem(lorentz2)
test_castem(mazars)
test_castem(mazars2)
test_castem(druckerprager)
test_castem(druckerprager2)
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
test_castem(plasticity)
test_castem(plasisocin)
test_castem(viscoplasisocin)
test_castem(chaboche)
test_castem(chaboche2)
test_castem(viscochaboche)
test_castem(monocrystal)
test_castem(monocrystal-2)
test_castem(implicitmonocrystal)
test_castem(implicitmonocrystal-2)
test_castem(monocrystal_dd_cfc)
test_castem(monocrystal_dd_cfc_numericaljacobian)
test_castem(monocrystal_dd_cfc_irradiation)
test_castem(monocrystal_dd_cc)
test_castem(monocrystal_dd_cc_numericaljacobian)
test_castem(monocrystal_dd_cc_irradiation)
test_castem(monocrystal_dd_cc_irradiation_numericaljacobian)
test_castem(polycrystals)
test_castem(polycrystals_dd_cc)
test_castem(polycrystals_dd_cfc)
test_castem(hayhurst)
test_castem(implicithayhurst)
test_castem(gurson)
test_castem(gursontvergaardneedlemanplasticflow)
test_castem(gursontvergaardneedlemanviscoplasticflow)
test_castem(ellipticcreep)
test_castem(burger)
test_castem(ageingburger)
test_castem(tvergaard)
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
test_castem(elasticity_log1D-uniaxialloading-1-1D)
test_castem(elasticity_log1D-uniaxialloading-2-1D)
test_castem(elasticity_log1D-uniaxialloading-3-1D)
test_castem(norton_log1D-uniaxialloading-1-1D)
test_castem(norton_log1D-uniaxialloading-2-1D)
test_castem(norton_log1D-uniaxialloading-3-1D)
test_castem(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-1-1D)
test_castem(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-2-1D)
test_castem(elasticity_LogarithmicStrain1DWrapper-uniaxialloading-3-1D)
test_castem(elasticity_LogarithmicStrain1DWrapper-thermalstrain)
test_castem(elasticity_LogarithmicStrain1DWrapper-thermalstrain2)
test_castem(norton_LogarithmicStrain1DWrapper-uniaxialloading-1-1D)
test_castem(norton_LogarithmicStrain1DWrapper-uniaxialloading-2-1D)
test_castem(norton_LogarithmicStrain1DWrapper-uniaxialloading-3-1D)
test_castem(elasticity_malls-uniaxialloading-1-1D)
test_castem(elasticity_malls-uniaxialloading-1-2D)
test_castem(elasticity_malls-uniaxialloading-1-3D)
test_castem(elasticity_malls-uniaxialloading-2-1D)
test_castem(elasticity_malls-uniaxialloading-2-2D)
test_castem(elasticity_malls-uniaxialloading-2-3D)
test_castem(elasticity_malls-uniaxialloading-3-1D)
test_castem(elasticity_malls-uniaxialloading-3-2D)
test_castem(elasticity_malls-uniaxialloading-3-3D)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-1D)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-1D)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-1D)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-2D)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-2D)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-2D)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_castem(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
test_castem(incompressiblemooneyrivlinbehaviour)
test_castem(incompressibleplanestressgornetdesmoratbehavioursheartesting)
test_castem(incompressibleplanestressgornetdesmoratbehaviouruniaxialtesting)
test_castem(compressibleneohookeanbehaviour)
test_castem(incompressibleplanestressmooneyrivlinbehaviouruniaxialtesting)
test_castem(incompressibleplanestressgornetdesmoratbehaviourbiaxialtesting)
test_castem(implicitsimomieheelastoplasticity)
test_castem(T91-satohtest)
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
test_castem(anistropiclemaitreviscoplasticbehaviour)
test_castem(tssf)
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

if(CMAKE_CONFIGURATION_TYPES)
  foreach(conf ${CMAKE_CONFIGURATION_TYPES})
    get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/castem/UMATExternalBehaviourDescription.cxx.in"
      ${PROJECT_BINARY_DIR}/mfront/tests/behaviours/castem/UMATExternalBehaviourDescription_${conf}.cxx @ONLY)
    add_executable(UMATExternalBehaviourDescription_${conf} EXCLUDE_FROM_ALL
      ${PROJECT_BINARY_DIR}/mfront/tests/behaviours/castem/UMATExternalBehaviourDescription_${conf}.cxx)
    add_test(NAME UMATExternalBehaviourDescription_${conf}
      COMMAND UMATExternalBehaviourDescription_${conf}
      CONFIGURATIONS ${conf})
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST UMATExternalBehaviourDescription_${conf}
        PROPERTY DEPENDS "MFrontCastemBehaviours"
    	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
    elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST UMATExternalBehaviourDescription_${conf}
        PROPERTY DEPENDS "MFrontCastemBehaviours")
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    target_link_libraries(UMATExternalBehaviourDescription_${conf}
      TFELGlossary TFELSystem TFELTests TFELException)
    add_dependencies(check UMATExternalBehaviourDescription_${conf})
  endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
else(CMAKE_CONFIGURATION_TYPES)
  get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
  configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/castem/UMATExternalBehaviourDescription.cxx.in"
    ${PROJECT_BINARY_DIR}/mfront/tests/behaviours/castem/UMATExternalBehaviourDescription.cxx @ONLY)
  add_executable(UMATExternalBehaviourDescription EXCLUDE_FROM_ALL
    ${PROJECT_BINARY_DIR}/mfront/tests/behaviours/castem/UMATExternalBehaviourDescription.cxx)
  add_test(NAME UMATExternalBehaviourDescription COMMAND UMATExternalBehaviourDescription)
  target_link_libraries(UMATExternalBehaviourDescription
    TFELGlossary TFELSystem TFELTests TFELException)
  set_tests_properties(UMATExternalBehaviourDescription
    PROPERTIES DEPENDS MFrontCastemBehaviours)
  add_dependencies(check UMATExternalBehaviourDescription)
endif(CMAKE_CONFIGURATION_TYPES)
