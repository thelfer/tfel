configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles-30.txt"
    PolyCrystalsAngles-30.txt @ONLY)

set(mfront_tests_SOURCES
  MaterialPropertiesBoundsCheck
  ExternalStateVariablesBoundsCheck
  Elasticity
  Elasticity2
  Elasticity3
  OrthotropicElastic
  OrthotropicElastic2
  OrthotropicCreep
  OrthotropicCreep_rk42
  OrthotropicCreep_rkCastem
  ImplicitOrthotropicCreep
  HosfordPerfectPlasticity
  JohnsonCook_s
  JohnsonCook_ssr
  JohnsonCook_ssrt
  RusinekKlepaczko_ssrt
  Plasticity
  DruckerPrager
  DruckerPragerEcroLin
  DruckerPragerEcroLinDefault
  Norton Norton2
  NortonRK DDIF2
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
  OrthotropicSaintVenantKirchhoffElasticity
  FiniteStrainSingleCrystal
  FiniteStrainSingleCrystal_NumericalJacobian
  FiniteRotationSmallStrainElasticity
  FiniteRotationSmallStrainElasticity2
  ComputeStressFreeExpansion
  IsotropicLinearSwelling
  IsotropicVolumeSwelling
  OrthotropicSwelling
  OrthotropicSwelling2
  AxialGrowth
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity2
  LogarithmicStrainElasticity3
  LogarithmicStrainNortonCreep
  ComputeThermalExpansion
  ComputeThermalExpansion2
  ComputeThermalExpansion3
  GenerateMTestFileOnFailure
  ParameterTest
  ParameterTest2
  ProfilerTest
  ProfilerTest2
  ProfilerTest3)
if((NOT WIN32) AND (NOT CYGWIN) AND (NOT MSYS) AND (NOT APPLE))
  list(APPEND mfront_tests_SOURCES
       AbaqusAxialGrowth
       AbaqusAxialGrowth2
       AbaqusOrthotropicSwelling	
       AbaqusOrthotropicSwelling2
       AbaqusOrthotropicElastic
       AbaqusOrthotropicElastic2)
endif((NOT WIN32) AND (NOT CYGWIN) AND (NOT MSYS) AND (NOT APPLE))
mfront_dependencies(MFrontAbaqusBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)

mfront_behaviour_check_library(MFrontAbaqusBehaviours  abaqus ${mfront_tests_SOURCES})
target_link_libraries(MFrontAbaqusBehaviours
  PRIVATE AbaqusInterface)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontAbaqusBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

function(test_abaqus test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  set(_EXPECT_ADDITIONAL_ENVIRONMENT )
  set(_ADDITIONAL_ENVIRONMENT )
  set(_EXPECT_REFERENCE_FILE)
  set(_REFERENCE_FILE )
  foreach(_ARG ${ARGN})
    if(_EXPECT_ADDITIONAL_ENVIRONMENT)
      if(_ADDITIONAL_ENVIRONMENT)
	message(FATAL_ERROR "test_abaqus: environment already set")
      endif(_ADDITIONAL_ENVIRONMENT)
      set(_ADDITIONAL_ENVIRONMENT "${_ARG}")
      set(_EXPECT_ADDITIONAL_ENVIRONMENT )
    elseif(_EXPECT_REFERENCE_FILE)
      if(_REFERENCE_FILE)
	message(FATAL_ERROR "test_abaqus: reference file already set")
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
	message(FATAL_ERROR "test_abaqus: unsupported option '${_ARG}'")
      endif()
    endif()
  endforeach(_ARG ${ARGN})
  if(_EXPECT_ADDITIONAL_ENVIRONMENT)
    message(FATAL_ERROR "test_abaqus: expected environment variable")
  endif()
  if(_EXPECT_REFERENCE_FILE)
    message(FATAL_ERROR "test_abaqus: expected reference file")
  endif()
  if(_REFERENCE_FILE)
    set(_REFERENCE_FILE "${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
  endif(_REFERENCE_FILE)
  foreach(rm ${IEEE754_ROUNDING_MODES})
    set(test "abaqus${test_arg}_${rm}_mtest")
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/abaqus${test_arg}.mtest")
    set(_mtest_args)
    list(APPEND _mtest_args --rounding-direction-mode=${rm} )
    list(APPEND _mtest_args --verbose=level0)
    list(APPEND _mtest_args --result-file-output=false)
    list(APPEND _mtest_args --@interface@=umat)
    list(APPEND _mtest_args --@library@="$<TARGET_FILE:MFrontAbaqusBehaviours>")
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
        DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/abaqus"
        COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
        DESTINATION "share/doc/mfront/tests/behaviours/abaqus"
        COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endfunction(test_abaqus)

test_abaqus(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_abaqus(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_abaqus(materialpropertiesboundscheck3)
test_abaqus(materialpropertiesboundscheck4)
test_abaqus(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_abaqus(materialpropertiesboundscheck6)

test_abaqus(externalstatevariablesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_abaqus(externalstatevariablesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_abaqus(externalstatevariablesboundscheck3)
test_abaqus(externalstatevariablesboundscheck4)
test_abaqus(externalstatevariablesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)

test_abaqus(elasticity)
test_abaqus(elasticity2)
if(HAVE_CASTEM)
  test_abaqus(elasticity3)
endif(HAVE_CASTEM)
test_abaqus(elasticity4)
test_abaqus(elasticity5)
test_abaqus(elasticity6)
test_abaqus(elasticity7)
test_abaqus(elasticity8)
test_abaqus(elasticity9)
test_abaqus(elasticity10)
test_abaqus(elasticity11)
test_abaqus(elasticity12)
test_abaqus(elasticity13)
test_abaqus(elasticity14)
test_abaqus(elasticity15)
test_abaqus(elasticity16)
test_abaqus(elasticity18)
test_abaqus(elasticity19)
test_abaqus(elasticity20)
test_abaqus(elasticity21)
test_abaqus(elasticity22)
test_abaqus(elasticity23)
test_abaqus(elasticity24)
test_abaqus(elasticity25)
test_abaqus(elasticity26)
test_abaqus(elasticity27)
test_abaqus(elasticity33)
test_abaqus(druckerprager)
test_abaqus(druckerprager2)
test_abaqus(mazars)
test_abaqus(norton)
test_abaqus(norton2)
test_abaqus(stressrelaxation)
test_abaqus(implicitnorton)
test_abaqus(implicitnorton2)
test_abaqus(implicitnorton3)
test_abaqus(implicitnorton_broyden)
test_abaqus(implicitnorton_broyden2)
test_abaqus(implicitnorton_levenbergmarquardt)
test_abaqus(implicitorthotropiccreep)
test_abaqus(plasticity)
test_abaqus(ellipticcreep)
test_abaqus(strainhardeningcreep)
test_abaqus(plasisocin)
test_abaqus(viscoplasisocin)
test_abaqus(chaboche)
test_abaqus(chaboche2)
test_abaqus(viscochaboche)
test_abaqus(monocrystal)
test_abaqus(monocrystal-2)
test_abaqus(implicitmonocrystal)
test_abaqus(implicitmonocrystal-2)
test_abaqus(monocrystal_dd_cfc)
test_abaqus(monocrystal_dd_cfc_numericaljacobian)
test_abaqus(monocrystal_dd_cfc_irradiation)
test_abaqus(monocrystal_dd_cc)
test_abaqus(monocrystal_dd_cc_numericaljacobian)
test_abaqus(monocrystal_dd_cc_irradiation)
test_abaqus(monocrystal_dd_cc_irradiation_numericaljacobian)
test_abaqus(tssf)
test_abaqus(implicithayhurst)
test_abaqus(burger)
test_abaqus(ageingburger)
test_abaqus(gursontvergaardneedlemanplasticflow)
test_abaqus(gursontvergaardneedlemanviscoplasticflow)
test_abaqus(anistropiclemaitreviscoplasticbehaviour)
test_abaqus(computestressfreeexpansion)
test_abaqus(isotropiclinearswelling)
test_abaqus(isotropicvolumeswelling)
test_abaqus(orthotropicswelling)
test_abaqus(orthotropicswelling2)
test_abaqus(axialgrowth)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
test_abaqus(saintvenantkirchhoffelasticity-shear-1-2D)
test_abaqus(saintvenantkirchhoffelasticity-shear-2-2D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_abaqus(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_abaqus(saintvenantkirchhoffelasticity-shear-1-3D)
test_abaqus(saintvenantkirchhoffelasticity-shear-2-3D)
test_abaqus(saintvenantkirchhoffelasticity-shear-3-3D)
test_abaqus(saintvenantkirchhoffelasticity-shear-4-3D)
test_abaqus(saintvenantkirchhoffelasticity-shear-5-3D)
test_abaqus(saintvenantkirchhoffelasticity-shear-6-3D)
test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-1-2D)
test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-2-2D)
test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-3-2D)
test_abaqus(saintvenantkirchhoffelasticity2-shear-1-2D)
test_abaqus(saintvenantkirchhoffelasticity2-shear-2-2D)
test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-1-3D)
test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-2-3D)
test_abaqus(saintvenantkirchhoffelasticity2-uniaxialloading-3-3D)
test_abaqus(saintvenantkirchhoffelasticity2-shear-1-3D)
test_abaqus(saintvenantkirchhoffelasticity2-shear-2-3D)
test_abaqus(saintvenantkirchhoffelasticity2-shear-3-3D)
test_abaqus(saintvenantkirchhoffelasticity2-shear-4-3D)
test_abaqus(saintvenantkirchhoffelasticity2-shear-5-3D)
test_abaqus(saintvenantkirchhoffelasticity2-shear-6-3D)
test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-1-2D)
test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-2-2D)
test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-3-2D)
test_abaqus(saintvenantkirchhoffelasticity3-shear-1-2D)
test_abaqus(saintvenantkirchhoffelasticity3-shear-2-2D)
test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-1-3D)
test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-2-3D)
test_abaqus(saintvenantkirchhoffelasticity3-uniaxialloading-3-3D)
test_abaqus(saintvenantkirchhoffelasticity3-shear-1-3D)
test_abaqus(saintvenantkirchhoffelasticity3-shear-2-3D)
test_abaqus(saintvenantkirchhoffelasticity3-shear-3-3D)
test_abaqus(saintvenantkirchhoffelasticity3-shear-4-3D)
test_abaqus(saintvenantkirchhoffelasticity3-shear-5-3D)
test_abaqus(saintvenantkirchhoffelasticity3-shear-6-3D)
test_abaqus(finiterotationsmallstrainelasticity-uniaxialloading-1-2D)
test_abaqus(finiterotationsmallstrainelasticity-uniaxialloading-2-2D)
test_abaqus(finiterotationsmallstrainelasticity-uniaxialloading-3-2D)
test_abaqus(finiterotationsmallstrainelasticity-shear-1-2D)
test_abaqus(finiterotationsmallstrainelasticity-shear-2-2D)
test_abaqus(finiterotationsmallstrainelasticity-uniaxialloading-1-3D)
test_abaqus(finiterotationsmallstrainelasticity-uniaxialloading-2-3D)
test_abaqus(finiterotationsmallstrainelasticity-uniaxialloading-3-3D)
test_abaqus(finiterotationsmallstrainelasticity-shear-1-3D)
test_abaqus(finiterotationsmallstrainelasticity-shear-2-3D)
test_abaqus(finiterotationsmallstrainelasticity-shear-3-3D)
test_abaqus(finiterotationsmallstrainelasticity-shear-4-3D)
test_abaqus(finiterotationsmallstrainelasticity-shear-5-3D)
test_abaqus(finiterotationsmallstrainelasticity-shear-6-3D)
test_abaqus(finiterotationsmallstrainelasticity2-uniaxialloading-1-2D)
test_abaqus(finiterotationsmallstrainelasticity2-uniaxialloading-2-2D)
test_abaqus(finiterotationsmallstrainelasticity2-shear-1-2D)
test_abaqus(finiterotationsmallstrainelasticity2-shear-2-2D)
test_abaqus(finiterotationsmallstrainelasticity2-uniaxialloading-1-3D)
test_abaqus(finiterotationsmallstrainelasticity2-uniaxialloading-2-3D)
test_abaqus(finiterotationsmallstrainelasticity2-uniaxialloading-3-3D)
test_abaqus(finiterotationsmallstrainelasticity2-shear-1-3D)
test_abaqus(finiterotationsmallstrainelasticity2-shear-2-3D)
test_abaqus(finiterotationsmallstrainelasticity2-shear-3-3D)
test_abaqus(finiterotationsmallstrainelasticity2-shear-4-3D)
test_abaqus(finiterotationsmallstrainelasticity2-shear-5-3D)
test_abaqus(finiterotationsmallstrainelasticity2-shear-6-3D)
test_abaqus(finitestrainsinglecrystal-001)
test_abaqus(finitestrainsinglecrystal-012)
test_abaqus(finitestrainsinglecrystal-012-2)
test_abaqus(finitestrainsinglecrystal-159)
test_abaqus(finitestrainsinglecrystal_numericaljacobian-001)
test_abaqus(finitestrainsinglecrystal_numericaljacobian-012)
test_abaqus(finitestrainsinglecrystal_numericaljacobian-012-2)
test_abaqus(finitestrainsinglecrystal_numericaljacobian-159)
if(NOT WIN32)
  test_abaqus(axialgrowth-omp)
  test_abaqus(elasticity14-omp)
  test_abaqus(elasticity6-omp)
  test_abaqus(elasticity10-omp)
  test_abaqus(elasticity22-omp)
  test_abaqus(orthotropicswelling2-omp)
  test_abaqus(elasticity11-omp)
  test_abaqus(elasticity23-omp)
  test_abaqus(orthotropicswelling-omp)
  test_abaqus(elasticity12-omp)
  test_abaqus(elasticity25-omp)
  test_abaqus(elasticity13-omp)
  test_abaqus(elasticity27-omp)
endif(NOT WIN32)
test_abaqus(orthotropicsaintvenantkirchhoffelasticity-3D-sig11)
test_abaqus(orthotropicsaintvenantkirchhoffelasticity-3D-sig11-2)
test_abaqus(orthotropicsaintvenantkirchhoffelasticity-3D-sig11-3)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-1-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-2-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-3-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity-shear-2D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity-shear-FXY-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity-shear-FXZ-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity-shear-FYX-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity-shear-FYZ-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity-shear-FZX-3D)
test_abaqus(mieheapellambrechtlogarithmicstrainelasticity-shear-FZY-3D)
if((NOT WIN32) AND (NOT CYGWIN) AND (NOT MSYS))
  test_abaqus(parametertest)
  test_abaqus(parametertest2)
endif((NOT WIN32) AND (NOT CYGWIN) AND (NOT MSYS))
