configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles-30.txt"
    PolyCrystalsAngles-30.txt @ONLY)

set(mfront_tests_SOURCES
  DSLOptionsTest
  InitializationFailureTest
  Elasticity
  Elasticity2
  Elasticity3
  OrthotropicElastic
  OrthotropicElastic2
  JohnsonCook_s
  JohnsonCook_ssr
  JohnsonCook_ssrt
  RusinekKlepaczko_ssrt
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity2
  LogarithmicStrainNortonCreep
  SaintVenantKirchhoffElasticity
  FiniteStrainSingleCrystal
  ComputeThermalExpansion
  ComputeThermalExpansion2
  ComputeThermalExpansion3
  ParameterTest
  ParameterTest2
  ProfilerTest
  ProfilerTest2
  ProfilerTest3)
mfront_dependencies(MFrontCalculiXBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)

mfront_behaviour_check_library(MFrontCalculiXBehaviours calculix
  ${mfront_tests_SOURCES})
target_link_libraries(MFrontCalculiXBehaviours
  PRIVATE CalculiXInterface)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontCalculiXBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

function(test_calculix test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  set(_EXPECT_ADDITIONAL_ENVIRONMENT )
  set(_ADDITIONAL_ENVIRONMENT )
  set(_EXPECT_REFERENCE_FILE)
  set(_REFERENCE_FILE )
  foreach(_ARG ${ARGN})
    if(_EXPECT_ADDITIONAL_ENVIRONMENT)
      if(_ADDITIONAL_ENVIRONMENT)
	message(FATAL_ERROR "test_calculix: environment already set")
      endif(_ADDITIONAL_ENVIRONMENT)
      set(_ADDITIONAL_ENVIRONMENT "${_ARG}")
      set(_EXPECT_ADDITIONAL_ENVIRONMENT )
    elseif(_EXPECT_REFERENCE_FILE)
      if(_REFERENCE_FILE)
	message(FATAL_ERROR "test_calculix: reference file already set")
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
	message(FATAL_ERROR "test_calculix: unsupported option '${_ARG}'")
      endif()
    endif()
  endforeach(_ARG ${ARGN})
  if(_EXPECT_ADDITIONAL_ENVIRONMENT)
    message(FATAL_ERROR "test_calculix: expected environment variable")
  endif()
  if(_EXPECT_REFERENCE_FILE)
    message(FATAL_ERROR "test_calculix: expected reference file")
  endif()
  if(_REFERENCE_FILE)
    set(_REFERENCE_FILE "${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/${_REFERENCE_FILE}")
  endif(_REFERENCE_FILE)
  foreach(rm ${IEEE754_ROUNDING_MODES})
    set(test "calculix${test_arg}_${rm}_mtest")
    set(file "${CMAKE_CURRENT_SOURCE_DIR}/calculix${test_arg}.mtest")
    set(_mtest_args)
    list(APPEND _mtest_args --rounding-direction-mode=${rm} )
    list(APPEND _mtest_args --verbose=level0)
    list(APPEND _mtest_args --result-file-output=false)
    list(APPEND _mtest_args --@interface@=umat)
    list(APPEND _mtest_args --@library@="$<TARGET_FILE:MFrontCalculiXBehaviours>")
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
        DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/calculix"
        COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${file}
        DESTINATION "share/doc/mfront/tests/behaviours/calculix"
        COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endfunction(test_calculix)

test_calculix(initializationfailuretest
  NO_XML_OUTPUT WILL_FAIL)

test_calculix(elasticity)
test_calculix(elasticity2)
if(HAVE_CASTEM)
  test_calculix(elasticity3)
endif(HAVE_CASTEM)
test_calculix(elasticity4)
test_calculix(elasticity5)
test_calculix(elasticity6)
test_calculix(elasticity7)
test_calculix(elasticity8)
test_calculix(elasticity9)
test_calculix(elasticity10)
test_calculix(elasticity11)
test_calculix(elasticity14)
test_calculix(elasticity15)
test_calculix(elasticity18)
test_calculix(elasticity19)
test_calculix(elasticity20)
test_calculix(elasticity21)
test_calculix(elasticity22)
test_calculix(elasticity23)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_calculix(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-1-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-2-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-3-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-4-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-5-3D)
test_calculix(saintvenantkirchhoffelasticity-shear-6-3D)

test_calculix(finitestrainsinglecrystal-001)
test_calculix(finitestrainsinglecrystal-012-2)
test_calculix(finitestrainsinglecrystal-012)
test_calculix(finitestrainsinglecrystal-159)

test_calculix(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_calculix(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_calculix(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_calculix(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_calculix(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_calculix(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_calculix(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_calculix(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_calculix(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)

test_calculix(parametertest)
test_calculix(parametertest2)
